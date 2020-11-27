#include <iostream>

using namespace std;

int main() {
    int i, j, v = 0;
    for (i=2; i<10; i++)
        for (j=0; j<5; j++)
	       v += i * j;
    std::cout << v << std::endl;
    return 0;
}
