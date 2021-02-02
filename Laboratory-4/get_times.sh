#!/bin/bash
g++ "smallpt_thread_pool.cpp" -o "smallpt_thread_pool" -O2 -I include -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11
g++ "test_with_queue.cpp" -o "test_with_queue" -O2 -I include -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11
g++ "pi_taylor_parallel_threadpool.cpp" -o "pi_taylor_parallel_threadpool" -O2 -I include -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11
g++ "smallpt_thread_pool_lock_free.cpp" -o "smallpt_thread_pool_lock_free" -O2 -I include -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11

# for n in 3 4 5 6 7 8
# do
#     echo $n
#     i=1
#     while [ $i -le 1 ]
#     do
#         ./smallpt_thread_pool $(( $n * $n )) 1
#         i=$(( $i + 1 ))
#     done
#     echo
#     i=1
#     while [ $i -le 1 ]
#     do
#         ./smallpt_thread_pool 1 $(( $n * $n ))
#         i=$(( $i + 1 ))
#     done
#     echo
#     i=1
#     while [ $i -le 1 ]
#     do
#         ./smallpt_thread_pool $n $n
#         i=$(( $i + 1 ))
#     done
# done

#for n in 1 2 4 6 8 10 12 16 20 32
for n in 1 2 3 4 5 6 7 8 9 10 11 12
do
    echo
    i=1
    while [ $i -le 5 ]
    do
        ./test_with_queue $n 1000000
        i=$(( $i + 1 ))
    done
done

# for n in 1 2 4 8 16
# do
#     echo
#     i=1
#     while [ $i -le 5 ]
#     do
#         ./pi_taylor_parallel_threadpool 4294967295 $n 
#         i=$(( $i + 1 ))
#     done
# done