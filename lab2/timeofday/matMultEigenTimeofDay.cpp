#include <iostream>
#include <Eigen/Dense>
#include <sys/time.h>

//#define EIGEN_DONT_VECTORIZE 1;

using namespace std;

int main(int argc, char const *argv[])
{
    struct timeval t0, t1, t2, t3;
    /*
        t0: Beggining of the program
        t1: Matrix multiplication starts
        t2: Matrix multiplication ends
        t3: The program ends
    */

    gettimeofday(&t0, NULL);

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
    gettimeofday(&t1, NULL);
    // Perform multiplication.
    c = a * b;
    gettimeofday(&t2, NULL);

    gettimeofday(&t3, NULL);

    time_t program_time_s = t3.tv_sec - t0.tv_sec;
    suseconds_t program_time_us = t3.tv_usec - t0.tv_usec;
    time_t mult_time_s = t2.tv_sec - t1.tv_sec;
    suseconds_t mult_time_us = t3.tv_usec - t0.tv_usec;

    cout << "ps " << program_time_s << "pus " << program_time_us << endl;
    cout << "ms " << mult_time_s << "mus " << mult_time_us << endl;

    return 0;
}
