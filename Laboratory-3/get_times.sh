g++ "pi_taylor_sequential.cc" -o "pi_taylor_sequential" -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11
g++ "pi_taylor_parallel.cc" -o "pi_taylor_parallel" -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11

for N in 16 128 1024 1048576 4294967295
do
    echo "Sequential N = $N"
    ./pi_taylor_sequential $N #>> times.txt
done
#perf stat ./pi_taylor_sequential 4294967295
for threads in 1 2 4 8 16
do
    echo "Parallel N = 4294967295 threads = $threads"
    ./pi_taylor_parallel 4294967295 $threads #>> times.txt
done
perf stat ./pi_taylor_parallel 4294967295 4
perf stat ./pi_taylor_parallel 4294967295 8