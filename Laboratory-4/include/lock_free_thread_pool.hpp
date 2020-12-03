#pragma once

#include <atomic>
#include <functional>
#include <vector>

#include<join_threads.hpp>
#include<lock_free_queue.hpp>

class lock_free_thread_pool
{
  std::atomic<bool> _done;
  const size_t _n_threads;
  using task_type = void();
  lock_free_queue<std::function<task_type>> _queue; 
  std::vector<std::thread> thread_vector;
  join_threads _joiner;

  void worker_thread()
  {
      while(!_done){
          std::function<task_type> task;
         
          if(_queue.try_pop(task)){
            task();
          }
          else{
             std::this_thread::yield();
          }
      }
  }

  public:
  lock_free_thread_pool(size_t num_threads = std::thread::hardware_concurrency())
    : _done(false), _n_threads(num_threads), _joiner(thread_vector)
  {
      for (size_t i = 0; i < _n_threads; i++)
      {
        thread_vector.push_back(std::thread(&lock_free_thread_pool::worker_thread, this)); 
      }
  }

  ~lock_free_thread_pool()
  {
    wait();
  }

  void wait()
  {
      // wait for completion
      _done = true;
      while(!_queue.empty()){
        std::this_thread::yield();
      }
  }

  template<typename F> 
  void submit(F f)
    {
       _queue.push(std::function<task_type>(f));
    }
};
