#include <iostream>

using namespace std;

void printMat(double **matrix, int rows, int columns)
{
    
    for(int i = 0; i < rows; i++)
    {
        cout << "[ ";
        for (int j = 0; j < columns; j++)
        {
            if (j == columns-1)
            {
                cout << matrix[i][j];
            } else {
                cout << matrix[i][j] << ", ";
            }
            
        }
        cout << " ]\n";
    }
}

int main(int argc, char const *argv[])
{
    int N;

    if (argc > 0)
    {
        N = atoi(argv[1]);
    } else {
        cout << "Please, introduce the size of square matrix N";
        return 0;
    }
    
    

    double **a, **b, **c;
    
    a = new double *[N];
    b = new double *[N];
    c = new double *[N];

    for (int i = 0; i < N; i++)
    {
        a[i] = new double [N];
        b[i] = new double [N];
        c[i] = new double [N];
    }

    double sum;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            a[i][j] = i + j;
            b[i][j] = i + j;
        }
    }

    //printMat(a, N, N);

    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            sum = 0;

            for (int n = 0; n < N; n++)
            {
                sum += a[row][n] * b[n][col];
            }
            c[row][col] = sum;
        }
    }

	return 0;
}
