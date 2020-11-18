g++ "pi_taylor_sequential.cc" -o "pi_taylor_sequential" -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11
g++ "pi_taylor_parallel.cc" -o "pi_taylor_parallel" -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11

for N in 16 128 1024 1048576 #4294967295
do
    echo "Sequential N = $N"
    ./pi_taylor_sequential $N #>> times.txt
done

for threads in 1 2 4 8 16
do
    echo "Parallel N = 1048576 threads = $threads"
    ./pi_taylor_parallel 1048576 $threads #>> times.txt
done