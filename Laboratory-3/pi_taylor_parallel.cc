#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using my_float = long double;

void pi_taylor_chunk(std::vector<my_float> &output, std::vector<std::chrono::nanoseconds> &time_thread,
                     size_t thread_id, size_t start_step, size_t stop_step){
    auto start = std::chrono::steady_clock::now();
    my_float sum = 0;
    int num = 1;
    int den;
    for (size_t i = start_step; i < stop_step; i++)
    {
        den = 2 * i + 1;
        sum += num / (my_float)den;
        num = -num;
    }
    output[thread_id] = sum;
    auto stop = std::chrono::steady_clock::now();
    time_thread[thread_id] = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
}

std::pair<size_t, size_t>
usage(int argc, const char *argv[])
{
    // read the number of steps from the command line
    if (argc != 3)
    {
        std::cerr << "Invalid syntax: pi_taylor <steps> <threads>" << std::endl;
        exit(1);
    }

    size_t steps = std::stoll(argv[1]);
    size_t threads = std::stoll(argv[2]);

    if (steps < threads)
    {
        std::cerr << "The number of steps should be larger than the number of threads" << std::endl;
        exit(1);
    }
    return std::make_pair(steps, threads);
}

typedef struct {
    size_t large_chunk;
    size_t small_chunk;
    size_t split_item;
} chunk_info;

// For a given number of iterations N and threads
// the iterations are divided:
// N % threads receive N / threads + 1 iterations
// the rest receive N / threads
constexpr chunk_info
split_evenly(size_t N, size_t threads)
{
    return {N / threads + 1, N / threads, N % threads};
}

std::pair<size_t, size_t>
get_chunk_begin_end(const chunk_info& ci, size_t index)
{
    size_t begin = 0, end = 0;
    if (index < ci.split_item ) {
        begin = index*ci.large_chunk;
        end = begin + ci.large_chunk; // (index + 1) * ci.large_chunk
    } else {
        begin = ci.split_item*ci.large_chunk + (index - ci.split_item) * ci.small_chunk;
        end = begin + ci.small_chunk;
    }
    return std::make_pair(begin, end);
}

int main(int argc, const char *argv[])
{

    auto ret_pair = usage(argc, argv);
    auto steps = ret_pair.first;
    auto threads = ret_pair.second;
    my_float pi;
    
    std::vector<my_float> results(threads);
    std::vector<std::chrono::nanoseconds> time_thread(threads);
    std::vector<std::thread> thread_vector;
    auto chunks = split_evenly(steps, threads);

    auto start = std::chrono::steady_clock::now();
    for(size_t i = 0; i < threads; ++i) {
        auto begin_end = get_chunk_begin_end(chunks, i);
        thread_vector.push_back(std::thread(pi_taylor_chunk, ref(results), ref(time_thread), i, 
                                            begin_end.first, begin_end.second));
    }

    // wait for completion
    for(size_t i = 0; i < threads; ++i) {
        thread_vector[i].join();
    }

    auto stop = std::chrono::steady_clock::now();

    std::chrono::nanoseconds time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);

    for (unsigned int i = 0; i < threads; i++){
        pi += results[i];
        std::cout << "Time thread " << i << ": " << time_thread[i].count() << std::endl;
    }
    pi *= 4;

    std::cout << "For " << steps << ", pi value: "
                << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
                << pi << std::endl;
    std::cout << "Time: " << time.count() << std::endl;
}
