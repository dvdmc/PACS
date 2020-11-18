#include <iomanip>
#include <iostream>
#include <limits>
#include <math.h>
#include <chrono>

// Allow to change the floating point type
using my_float = long double;

my_float pi_taylor(size_t steps) {
    my_float sum = 0;
    int num = 1;
    my_float den;
    for (size_t i = 0; i < steps; i++){
        den = 2 * i + 1;
        sum += num/den;
        num = -num;
    }
    return 4 * sum;
}

int main(int argc, const char *argv[]) {

    // read the number of steps from the command line
    if (argc != 2) {
        std::cerr << "Invalid syntax: pi_taylor <steps>" << std::endl;
        exit(1);

    }

    size_t steps = std::stoll(argv[1]); //stoll: string to long long
    
    auto start = std::chrono::steady_clock::now();
    auto pi = pi_taylor(steps);
    auto stop = std::chrono::steady_clock::now();

    std::chrono::nanoseconds time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
    std::cout << std::setprecision(std::numeric_limits<my_float>::digits10 + 1) << pi << " " << time.count() << std::endl;
}
