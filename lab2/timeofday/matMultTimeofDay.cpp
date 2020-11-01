#include <iostream>
#include <sys/time.h>

using namespace std;

void printMat(double *matrix, int rows, int columns)
{

    for (int i = 0; i < rows; i++)
    {
        cout << "[ ";
        for (int j = 0; j < columns; j++)
        {
            if (j == columns - 1)
            {
                cout << matrix[i * columns + j];
            }
            else
            {
                cout << matrix[i * columns + j] << ", ";
            }
        }
        cout << " ]\n";
    }
}

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

    double *a, *b, *c;

    a = new double[N * N];
    b = new double[N * N];
    c = new double[N * N];

    double *ap = a;
    double *bp = b;
    double *cp = c;

    double sum;

    // Init matrices with some values.
    for (int i = 0; i < N * N; i++)
    {
        *(ap++) = i * 2;
        *(bp++) = i * 2;
    }

    // Reposition pointers.
    ap = a;
    bp = b;
    // Create a temporal pointer for the multiplication process.
    double *tap = a;

    gettimeofday(&t1, NULL);

    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            // Return tap to the initial column.
            tap = ap;

            // Initialize a new sum.
            sum = 0;

            for (int n = 0; n < N; n++)
            {
                // Multiply and add to the sum. Then increase a column in A...
                sum += *(tap++) * *bp;
                // And a row in b.
                bp += N;
            }
            // Return to the initial row and increment column.
            bp = bp - N * N + 1;

            // Set the result and move position.
            *(cp++) = sum;
        }

        // Return bp to it's initial position.
        bp = b;
        // Increment row.
        ap += N;
    }

    gettimeofday(&t2, NULL);

    // This is done so O2 don't delete the operation.
	sum = c[N*N-1];
	delete[] a;
	delete[] b;
	delete[] c;

    gettimeofday(&t3, NULL);

    time_t program_time_s = t3.tv_sec - t0.tv_sec;
    suseconds_t program_time_us = t3.tv_usec - t0.tv_usec;
    time_t mult_time_s = t2.tv_sec - t1.tv_sec;
    suseconds_t mult_time_us = t3.tv_usec - t0.tv_usec;

    cout << "ps " << program_time_s << "pus " << program_time_us << endl;
    cout << "ms " << mult_time_s << "mus " << mult_time_us << endl;
    return sum;
}
