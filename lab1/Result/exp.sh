#!/bin/bash

g++ -Wall matMult.cpp -o matMult -O2

g++ -Wall matMultEigen.cpp -I /usr/local/include/eigen3 -o matMultEigen -O2

for dim in 5 10 50 100 500 1000
do
	echo "Dimension of the matrix: $dim" >> for_mult.txt
	(time ./matMult $dim) &>> for_mult.txt
	echo "Dimension of the matrix: $dim" >> my_eigen_matmult.txt
	(time ./matMultEigen $dim) &>> my_eigen_matmult.txt
done