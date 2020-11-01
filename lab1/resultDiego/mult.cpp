#include <random>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
	// Initialize a random seed
	srand((unsigned)time(NULL));
	// Get the dimension as a parameter
	int dim = atoi(argv[1]);

	// Allocate the necessary memory
    double** matrix1 = new double*[dim];
	double** matrix2 = new double*[dim];
	double** sol = new double*[dim];
	for (int i = 0; i < dim; i++){
		matrix1[i] = new double[dim];
		matrix2[i] = new double[dim];
		sol[i] = new double[dim];
	}

	// Fill the matrixes with random doubles
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			matrix1[i][j] = (double)rand();
			matrix2[i][j] = (double)rand();
		}
	}

	// Perform the multiplication
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){		
			sol[i][j] = 0;
			for (int n = 0; n < dim; n++){
				sol[i][j] += matrix1[i][n]*matrix2[n][j];
			}
		}
	}

	// Free the memory
	for (int i = 0; i < dim; ++i){
    	delete matrix1[i];
		delete matrix2[i];
		delete sol[i];
	}
	delete matrix1;
	delete matrix2;
	delete sol;
    return 0;
}
