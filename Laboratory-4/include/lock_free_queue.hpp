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
                if (tail == this->Tail){ //Check if other thread has push a node
                    if (next == nullptr){ //Check if the pointer tail points to the last node (i.e. its next is nullptr)
                        if (std::atomic_compare_exchange_weak(&tail->next, &next, node)){ 
                            done = true;
                        }
                    }
                    else
                    {
                        //Make this->Tail point to the last element if no other thread has make it before
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


        std::shared_ptr<T> wait_and_pop()
        {
            bool done = false;
            std::shared_ptr<node_t> head, tail, next;
            T value;
            while(!done){
                head = this->Head;
                tail = this->Tail;
                next = head->next;
                if (head == this->Head){
                    if (head == tail){
                        if (next != nullptr){
                            std::atomic_compare_exchange_weak(&this->Tail, &tail, next);
                        }  
                    }
                    else{
                        value = next->value;
                        if (std::atomic_compare_exchange_weak(&this->Head, &head, next)){
                            done = true;
                        }
                    }
                }
            }
            return std::make_shared<T>(value);
        }


        void wait_and_pop(T& value)
        {
            bool done = false;
            std::shared_ptr<node_t> head, tail, next;
            while(!done){
                head = this->Head;
                tail = this->Tail;
                next = head->next;
                if (head == this->Head){
                    if (head == tail){
                        if (next != nullptr){
                            std::atomic_compare_exchange_weak(&this->Tail, &tail, next);
                        }   
                    }
                    else{
                        value = next->value;
                        if (std::atomic_compare_exchange_weak(&this->Head, &head, next)){
                            done = true;
                        }
                    }
                }
            }
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
