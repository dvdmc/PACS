g++ "pi_taylor_sequential.cc" -o "pi_taylor_sequential" -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11
g++ "pi_taylor_parallel.cc" -o "pi_taylor_parallel" -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11
g++ "pi_taylor_parallel_kahan.cc" -o "pi_taylor_parallel_kahan" -Wall -Wextra -pedantic -Werror -pthread -std=gnu++11


#for N in 16 128 1024 1048576 4294967295
#for N in 100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000
#do
#    echo "Sequential N = $N"
#    ./pi_taylor_sequential $N #>> times.txt
#done
#perf stat ./pi_taylor_sequential 4294967295
for threads in 4 32
do
    echo cambiooooooooooooo
    i=1
    while [ $i -le 50 ]
    do
        ./pi_taylor_parallel 1048576 $threads #>> times.txt
        i=$(( $i + 1 ))
    done
done