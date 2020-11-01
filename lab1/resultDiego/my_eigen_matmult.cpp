#include <random>
#include <Eigen/Dense>
#include <cstdlib>

using Eigen::MatrixXd;
using namespace std;

int main(int argc, char *argv[]) {
	// Initialize the random seed
	srand((unsigned)time(NULL));
	// Get the dimension as a parameter
	int dim = atoi(argv[1]);

	// Initializa the matrixes dinamically
	MatrixXd matrix1(dim, dim), matrix2(dim,dim), sol(dim,dim);

	// Fill the matrixes with random values
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			matrix1(i,j) = (double)rand();
			matrix2(i,j) = (double)rand();
		}
	}

	// Perform the multiplication
	sol = matrix1 * matrix2;
    return 0;
}
