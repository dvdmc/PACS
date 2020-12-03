#pragma once

#include <atomic>
#include <functional>
#include <vector>
#include <future>

#include<join_threads.hpp>
#include<threadsafe_queue_pack.hpp>

class thread_pool_pack
{
  std::atomic<bool> _done;
  const size_t _n_threads;
  //using task_type = std::function<void()>;
  using my_float = long double;
  using task_type = std::packaged_task<my_float()>;
  threadsafe_queue_pack<task_type> _queue; 
  std::vector<std::thread> thread_vector;
  join_threads _joiner;

  void worker_thread()
  {
      while(!_done){
          task_type task;
         
          if(_queue.try_pop(task)){
            task();
          }
          else{
             std::this_thread::yield();
          }
      }
  }

  public:
  thread_pool_pack(size_t num_threads = std::thread::hardware_concurrency())
    : _done(false), _n_threads(num_threads), _joiner(thread_vector)
  {
      for (size_t i = 0; i < _n_threads; i++)
      {
        thread_vector.push_back(std::thread(&thread_pool_pack::worker_thread, this)); 
      }
  }

  ~thread_pool_pack()
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
       _queue.push(std::move(f));
    }
};
