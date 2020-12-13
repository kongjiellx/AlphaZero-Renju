#include "cpp/src/utils/dirichlet.h"
#include <vector>
#include "iostream"

int main() {
    // initialize rng
    std::random_device rd;
    std::mt19937 gen(rd());

    // Dirichlet distribution using mt19937 rng
    dirichlet_distribution<std::mt19937> d(std::vector<double>(100, 1));

    // print 10 random draws
    for (int i = 0; i < 10; ++i) {
        for (double x : d(gen)) std::cout << x << " ";
        std::cout << std::endl;
    }
    return 0;
}