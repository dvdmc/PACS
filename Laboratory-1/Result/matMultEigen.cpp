#include <iostream>
#include <Eigen/Dense>

//#define EIGEN_DONT_VECTORIZE 1;
using namespace std;

int main(int argc, char const *argv[])
{
    int N;

    N = atoi(argv[1]);

    // Initialize the matrices dinamically
    Eigen::MatrixXd a(N, N), b(N, N), c(N, N);

    // Init matrices with some values.
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            a(i, j) = (i + j) * 2;
            b(i, j) = (i + j) * 2;
        }
    }
    
    // Perform multiplication.
    c = a * b;
    return 0;
}
