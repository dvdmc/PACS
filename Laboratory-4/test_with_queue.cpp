#include <atomic>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <lock_free_queue.hpp>
#include <threadsafe_queue.hpp>

template<typename Queue>
void push_values (Queue& q, size_t n_iterations){
    for (size_t i = 0; i < n_iterations; i++){
        q.push((int)i);
    }
}

template<typename Queue>
void pop_values (Queue& q, size_t n_iterations){
    int aux;
    for (size_t i = 0; i < n_iterations; i++){
        while(!q.try_pop(aux)){ }
    }
}

std::pair<size_t, size_t>
usage(int argc, char *argv[]) {
    if (!((argc == 1) || (argc == 3))) {
        std::cerr << "Invalid syntax: test_with_queue <n_threads_each_type> <n_iterations>" << std::endl;
        exit(1);
    }

    size_t n_threads = argc == 1 ? 2 : std::stol(argv[1]);
    size_t n_iterations = argc == 1 ? 2 : std::stol(argv[1]);

    if ((n_threads < 1) || (n_iterations < 1)){
        std::cerr << "The minimum number of threads and iterations is 1" << std::endl;
        exit(1);
    }
    return std::make_pair(n_threads, n_iterations);
}

int main(int argc, char *argv[]){
    auto p = usage(argc, argv);
    auto n_threads2 = p.first;
    auto n_iterations = p.second;
    std::vector<std::thread> thread_vector;

    auto start = std::chrono::steady_clock::now();
    threadsafe_queue<int> q_ts;
    for(size_t i = 0; i < n_threads2; ++i) {
        thread_vector.push_back(std::thread(push_values<threadsafe_queue<int>>, std::ref(q_ts), n_iterations));
        thread_vector.push_back(std::thread(pop_values<threadsafe_queue<int>>, std::ref(q_ts), n_iterations));
    }
    // wait for completion
    for(size_t i = 0; i < n_threads2*2; ++i) {
        thread_vector[i].join();
    }
    auto stop = std::chrono::steady_clock::now();
    std::chrono::nanoseconds time_threadsafe = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);

    thread_vector.clear();

    start = std::chrono::steady_clock::now();
    lock_free_queue<int> q_lf;
    for(size_t i = 0; i < n_threads2; ++i) {
        thread_vector.push_back(std::thread(push_values<lock_free_queue<int>>, std::ref(q_lf), n_iterations));
        thread_vector.push_back(std::thread(pop_values<lock_free_queue<int>>, std::ref(q_lf), n_iterations));
    }
    // wait for completion
    for(size_t i = 0; i < n_threads2*2; ++i) {
        thread_vector[i].join();
    }
    stop = std::chrono::steady_clock::now();
    std::chrono::nanoseconds time_lock_free = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
    std::cout << time_threadsafe.count() << std::endl;
    std::cout << time_lock_free.count() << std::endl;
}
