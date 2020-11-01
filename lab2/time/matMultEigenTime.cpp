#include <iostream>
#include <Eigen/Dense>
#include <ctime>
#include <iomanip>

//#define EIGEN_DONT_VECTORIZE 1;
using namespace std;

int main(int argc, char const *argv[])
{
    clock_t time;
    time = clock();

    int N;

    if (argc > 1)
    {
        N = atoi(argv[1]);
    }
    else
    {
        cout << "Please, introduce the size of square matrix N";
        return 0;
    }

    Eigen::MatrixXd a(N, N), b(N, N), c(N, N);

    // Init matrices with some values.
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            a(i, j) = i * 2;
            b(i, j) = i * 2;
        }
    }
    // Perform multiplication.
    c = a * b;

    time = clock() - time;
    cout << "Seconds: " << fixed << setprecision(4) << ((float)time)/CLOCKS_PER_SEC << endl;
    
    return 0;
}
