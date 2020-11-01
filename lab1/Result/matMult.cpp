#include <iostream>

using namespace std;

// Helper function to print a matrix and check results.
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
    int N;

    N = atoi(argv[1]);

    // Initialize the matrices dinamically as strided rows
    // to locate all values close to each other in memory.
    double *a, *b, *c;

    a = new double[N * N];
    b = new double[N * N];
    c = new double[N * N];

    double *ap = a;
    double *bp = b;
    double *cp = c;

    // Init matrices with some values.
    for (int i = 0; i < N * N; i++)
    {
        *(ap++) = i * 2;
        *(bp++) = i * 2;
    }

    // Reposition pointers.
    ap = a;
    bp = b;

    // Create temporal pointers for the multiplication process.
    double *tap = a;
    double *tbp = b;

    double sum;
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            // Return tap to the initial column.
            tap = ap;
            // Set bp to the initial row and next column.
            tbp = bp;
            // Initialize a new sum.
            sum = 0;

            for (int n = 0; n < N; n++)
            {
                // Multiply and add to the sum. Then increase a column in A...
                sum += *(tap++) * *tbp;
                // And a row in b.
                tbp += N;
            }
            // Iincrement column.
            bp++;

            // Set the result and move position.
            *(cp++) = sum;
        }

        // Return bp to it's initial position.
        bp = b;
        // Increment row.
        ap += N;
    }

    return 0;
}
