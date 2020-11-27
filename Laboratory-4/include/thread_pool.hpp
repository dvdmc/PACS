#pragma once

#include <atomic>
#include <functional>
#include <vector>

#include<join_threads.hpp>
#include<threadsafe_queue.hpp>

class thread_pool
{
  std::atomic<bool> _done;
  const size_t _n_threads;
  using task_type = void();
  threadsafe_queue<std::function<task_type>> _queue; //Pregunta: se puede usar directamente threadsafe_queue<task_type> ? Por qué std::function wrapper?
  std::vector<std::thread> thread_vector;
  join_threads _joiner;

  void worker_thread()
  {
      while(!_done){
          // TODO: Implement with _queue.wait_and_pop() and empty tasks for cleanup
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
  thread_pool(size_t num_threads = std::thread::hardware_concurrency())
    : _done(false), _n_threads(num_threads), _joiner(thread_vector)
  {
      for (size_t i = 0; i < _n_threads; i++)
      {
        thread_vector.push_back(std::thread(&thread_pool::worker_thread, this)); //Pregunta: se podría poner directamente worker_thread?
      }
  }

  ~thread_pool()
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

  template<typename F> //Pregunta:POR QUE F Y NO TASK TYPE: sentido de castear la funcion ??
  void submit(F f)
    {
       _queue.push(std::function<task_type>(f));
    }
};
