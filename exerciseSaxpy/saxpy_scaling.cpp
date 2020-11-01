#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
// assume a container
template <typename T>
void saxpy(T &z, const typename T::value_type A, const T &x, const T &y,
           size_t begin, size_t end)
{
    for (size_t i = begin; i < end; ++i)
    {
        z[i] = A * x[i] + y[i];
    }
}

int main()
{

    size_t N = 1024 * 1024 * 1024;
    const float A = 3.14f;
    using vf = std::vector<float>;
    vf z(N, 0.0f);
    vf x, y;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    std::vector<std::thread> thread_vector;
    std::vector<std::chrono::nanoseconds> extime_thread;
    std::vector<int> extime_iteration;

    size_t max_threads = std::min(48u, std::thread::hardware_concurrency());

    int N_min = 1;
    int N_max = 1000000;
    int median;

    while (N_max > N_min)
    {
        extime_thread.clear();
        N = (N_max + N_min) / 2 + 1;
        for (size_t i = 0; i < N; ++i)
        {
            float value = dis(gen);
            x.push_back(value);
            y.push_back(value); // reduce execution time
        }
        for (size_t current_threads = 1; current_threads <= 8; ++current_threads)
        {
            // ToDo : run several times and check median and deviation
            // launch the work
            for (size_t iter = 0; iter < 10; iter++)
            {
                auto start = std::chrono::steady_clock::now();
                for (size_t i = 0; i < current_threads - 1; ++i)
                {
                    size_t begin = i * (N / current_threads);
                    size_t end = begin + N / current_threads;
                    thread_vector.push_back(std::thread(saxpy<vf>, std::ref(z), A,
                                                        std::ref(x), std::ref(y), begin, end));
                }
                size_t begin = (current_threads - 1) * (N / current_threads);
                size_t end = std::max(N, begin + N / current_threads);
                thread_vector.push_back(std::thread(saxpy<vf>, std::ref(z), A,
                                                    std::ref(x), std::ref(y), begin, end));

                // wait for completion
                for (size_t i = 0; i < current_threads; ++i)
                {
                    thread_vector[i].join();
                }
                // clean the vector array
                thread_vector.clear();

                auto stop = std::chrono::steady_clock::now();
                //std::cout << current_threads << " " << (stop - start).count() << std::endl;
                extime_iteration.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count());
            }

            std::sort(extime_iteration.begin(), extime_iteration.end());
            median = extime_iteration[extime_iteration.size() / 2];

            double sum = std::accumulate(extime_iteration.begin(), extime_iteration.end(), 0.0);
            double mean = sum / extime_iteration.size();

            std::vector<double> diff(extime_iteration.size());
            std::transform(extime_iteration.begin(), extime_iteration.end(), diff.begin(), [mean](double x) { return x - mean; });
            double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
            double stdev = std::sqrt(sq_sum / extime_iteration.size());

            extime_iteration.clear();

            //std::cout << "Mean: " << mean << " STD: " << stdev << " Median: " << median << std::endl;

            extime_thread.push_back(std::chrono::nanoseconds(int(mean)));
        }

        if (extime_thread[0].count() > extime_thread[7].count())
        {
            N_max = N;
        }
        else
        {
            N_min = N;
        }
    }

    std::cout << "N max: " << N_max << "  N min: " << N_min << "   N:" << N;

    std::cout << "threads, time (ms)" << std::endl;
    for (size_t i = 0; i < extime_thread.size(); ++i)
    {
        std::cout << i + 1 << ", " << extime_thread[i].count() << std::endl;
    }
}
