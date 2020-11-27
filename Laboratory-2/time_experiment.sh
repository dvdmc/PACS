#!/bin/bash
g++ "lab2/time/matMultTime.cpp" -o "lab2/time/matMultTime" -O2 -Wall
g++ "lab2/time/matMultEigenTime.cpp" -o "lab2/time/matMultEigenTime" -O2 -I ./eigen-3.3.7 -Wall
g++ "lab2/timeofday/matMultTimeofDay.cpp" -o "lab2/timeofday/matMultTimeofDay" -O2 -Wall
g++ "lab2/timeofday/matMultEigenTimeofDay.cpp" -o "lab2/timeofday/matMultEigenTimeofDay" -O2 -I ./eigen-3.3.7 -Wall

for i in 100 500 750 1000 2000 4000
do
	echo $i
	# matMultTime
	echo "N value: $i" >> results/lab2/matMultTime.txt
	(time ./lab2/time/matMultTime $i) &>> results/lab2/matMultTime.txt
	echo "---" >> results/lab2/matMultTime.txt
	# matMultEigenTime
	echo "N value: $i" >> results/lab2/matMultEigenTime.txt
	(time ./lab2/time/matMultEigenTime $i) &>> results/lab2/matMultEigenTime.txt
	echo "---" >> results/lab2/matMultEigenTime.txt
	# matMultTimeofDay
	echo "N value: $i" >> results/lab2/matMultTimeofDay.txt
	(time ./lab2/timeofday/matMultTimeofDay $i) &>> results/lab2/matMultTimeofDay.txt
	echo "---" >> results/lab2/matMultTimeofDay.txt
	# matMultEigenTimeofDay
	echo "N value: $i" >> results/lab2/matMultEigenTimeofDay.txt
	(time ./lab2/timeofday/matMultEigenTimeofDay $i) &>> results/lab2/matMultEigenTimeofDay.txt
	echo "---" >> results/lab2/matMultEigenTimeofDay.txt
done