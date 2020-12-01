#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template<typename T>
class threadsafe_queue_pack
{
  private:
      mutable std::mutex _mtx;
      std::queue<T> q;
      std::condition_variable cond;

  public:
    threadsafe_queue_pack() { }

    threadsafe_queue_pack(const threadsafe_queue_pack& other)
    {
        // Acquire the mutex from the other queue to safely copy the queue.
        std::lock_guard<std::mutex> lk(other._mtx);
	    q = other.q;
    }

    threadsafe_queue_pack& operator=(const threadsafe_queue_pack&) = delete;

    void push(T new_value)
    {
	    std::lock_guard<std::mutex> lk(_mtx);
        q.push(std::move(new_value));
        cond.notify_one();
    }

    bool try_pop(T& value)
    {
	    std::lock_guard<std::mutex> lk(_mtx);
        
        if (q.empty()){
            return false;
        }
        else{
            value = std::move(q.front());
            q.pop();
            
            return true;
        }
    }

    void wait_and_pop(T& value)
    {
	    std::unique_lock<std::mutex> lk(_mtx);
        cond.wait(lk,[this]{return !q.empty();});
        value = std::move(q.front());
        q.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
	    std::unique_lock<std::mutex> lk(_mtx);
        cond.wait(lk,[this]{return !q.empty();});
        std::shared_ptr<T> ptr(std::make_shared<T>(q.front()));
        q.pop();
        return ptr;
    }

    bool empty() const
    {
	    std::lock_guard<std::mutex> lk(_mtx);
        return q.empty();
    }
};
