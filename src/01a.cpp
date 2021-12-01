#include <fstream>
#include <iostream>

int main() {
    std::ifstream ifs("inputs/01a.txt");
    if (!ifs.is_open()) {
        std::cerr << "file not found" << std::endl;
        return 1;
    }

    auto incs = 0;
    int prev, curr;

    ifs >> prev;
    while (ifs >> curr) {
        if (curr > prev) {
            incs++;
        }
        prev = curr;
    }
    
    std::cout << incs << std::endl;
    return 0;
}
