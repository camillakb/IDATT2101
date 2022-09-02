#include <iostream>
#include <stdlib.h>
#include <chrono>

//Finding the value of x^n.
double findxPown(double x, int n) {
    if (n == 0) {
        return 1.;
    }

    return x * findxPown(x, n - 1);
}

int main(int argc, char *argv[]) {
    auto a_second = std::chrono::duration<int>(1);
    auto start = std::chrono::high_resolution_clock::now();
    int counter = 0;

    //Finding the number of times the findxPown algorithm can be run in a second.
    while (std::chrono::high_resolution_clock::now() - start < a_second) {
        findxPown(1.0001, 100000);
        counter ++;
    }

    //Finding the average runtime of the algorithm.
    auto freq = std::chrono::duration<double>(1./counter);
    auto timeUsed = std::chrono::duration_cast<std::chrono::nanoseconds>(freq);

    std::cout << "x^n is: " << findxPown(1.0001, 100000) << std::endl;
    std:: cout << "Time used: " << timeUsed.count() << " ns" << std::endl;
}