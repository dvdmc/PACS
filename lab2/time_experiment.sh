#!/bin/bash
g++ matMult.cpp -o matMult -O2
g++ matMultEigen.cpp -o matMultEigen -O2 -I "/usr/local/include/eigen3"
g++ matMultTime.cpp -o matMultTime -O2
g++ matMultEigenTime.cpp -o matMultEigenTime -O2 -I "/usr/local/include/eigen3"

for i in 50 100 250 500 750 1000
do
  echo "N value: $i"
  echo "MatMult:"
  time ./matMult $i 
  echo "---"
done
echo "---/---"
for i in 50 100 250 500 750 1000
do
  echo "N value: $i"
  echo "Eigen:"
  time ./matMultEigen $i
  echo "---"
done