#include <array>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream ifs("inputs/01a.txt");
    if (!ifs.is_open()) {
        std::cerr << "file not found" << std::endl;
        return 1;
    }

    auto incs = 0;
    auto idx = 0;
    int curr;
    std::array<int, 3> prev;

    ifs >> prev[0] >> prev[1] >> prev[2];
    while (ifs >> curr) {
        if (curr > prev[idx]) {
            incs++;
        }
        prev[idx] = curr;
        idx = (idx + 1) % 3;
    }
    
    std::cout << incs << std::endl;
    return 0;
}
