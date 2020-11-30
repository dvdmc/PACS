#pragma once

#include <atomic>
#include <memory>

template<typename T>
class lock_free_queue
{
    private:
        struct node_t
        {
            T value;
            std::shared_ptr<node_t> next;
        };
        
        std::shared_ptr<node_t> Head;
        std::shared_ptr<node_t> Tail;

    public:
        lock_free_queue() 
        { 
            std::shared_ptr<node_t> node  (new node_t);
            node->next = nullptr;
            this->Head = node;
            this->Tail = node;
        }
        /*
        lock_free_queue(const lock_free_queue& other)
        {
            // Acquire the mutex from the other queue to safely copy the queue.
            std::lock_guard<std::mutex> lk(other._mtx);
            q = other.q;
        }*/

        lock_free_queue& operator=(const lock_free_queue&) = delete;

        void push(T new_value)
        {
            std::shared_ptr<node_t> node (new node_t);
            node->value = new_value;
            node->next = nullptr;
            bool done = false;
            std::shared_ptr<node_t> tail;
            while(!done){
                tail = this->Tail;
                std::shared_ptr<node_t> next = tail->next;
                if (tail == this->Tail){
                    if (next == nullptr){
                        if (std::atomic_compare_exchange_weak(&tail->next, &next, node)){ 
                            done = true;
                        }
                    }
                    else{
                        std::atomic_compare_exchange_weak(&this->Tail, &tail, next);
                    }
                }
            }
            std::atomic_compare_exchange_weak(&this->Tail, &tail, node);
        }

        bool try_pop(T& value)
        {
            bool done = false;
            std::shared_ptr<node_t> head, tail, next;
            while(!done){
                head = this->Head;
                tail = this->Tail;
                next = head->next;
                if (head == this->Head){
                    if (head == tail){
                        if (next == nullptr){
                            return false;
                        }
                        std::atomic_compare_exchange_weak(&this->Tail, &tail, next);
                    }
                    else{
                        value = next->value;
                        if (std::atomic_compare_exchange_weak(&this->Head, &head, next)){
                            done = true;
                        }
                    }
                }
            }
            return true;
        }

        bool empty()
        {
            std::shared_ptr<node_t> head, tail, next;
            while(true){
                head = this->Head;
                tail = this->Tail;
                next = head->next;
                if (head == this->Head){
                    if (head == tail){
                        if (next == nullptr){
                            return true;
                        }
                        std::atomic_compare_exchange_weak(&this->Tail, &tail, next);
                    }
                    else{
                        return false;
                    }
                }
            }
        }
};