#include <future>
#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include <functional>
#include <future>

// assume a container
template <typename T>
T saxpy(const typename T::value_type A, const T& x, const T& y,
        size_t begin, size_t end)
{
    T z(x.size());

    for(size_t i = begin; i < end; ++i) {
         z[i] = A * x[i] + y[i];
    }
    return z;
}

int main() {

    const size_t N = 100000;
    const float A = 3.14f;
    using vf = std::vector<float>;
    vf z(N, 0.0f);
    vf x, y;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for(size_t i = 0; i < N; ++i) {
        x.push_back(dis(gen));
        y.push_back(dis(gen));
    }
    auto start = std::chrono::steady_clock::now();
    auto first_half =  std::async(std::launch::deferred, saxpy<vf>, A, std::ref(x), std::ref(y), 0, N/2);
    auto second_half = std::async(std::launch::async, saxpy<vf>, A, std::ref(x), std::ref(y), N/2, N);
   

    /*
    std::cout << "x:";
    for(const auto i: x) {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "y:";
    for(const auto i: y) {
        std::cout << " " << i;
    }
    std::cout << std::endl;

    std::cout << "z:";
    for(const auto i: first_half.get()) {
        std::cout << " " << i;
    }
    for(const auto i: second_half.get()) {
        std::cout << " " << i;
    }
    */
    first_half.get();
    second_half.get();
    auto stop = std::chrono::steady_clock::now();
    float time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    
    std::cout << "TIME: " << time << std::endl;
    
    
    start = std::chrono::steady_clock::now();
    std::packaged_task<vf(float, vf, vf, size_t, size_t)> task_first_half(saxpy<vf>);
    std::packaged_task<vf(float, vf, vf, size_t, size_t)> task_second_half(saxpy<vf>);

    first_half = task_first_half.get_future();
    second_half= task_second_half.get_future();

    std::thread second_thread(std::move(task_second_half), A, std::ref(x), std::ref(y), N/2, N);
    task_first_half(A, std::ref(x), std::ref(y), 0, N/2);
    second_thread.join();
    first_half.get();
    second_half.get(); 
    stop = std::chrono::steady_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    
    std::cout << "TIME: " << time << std::endl;
}
