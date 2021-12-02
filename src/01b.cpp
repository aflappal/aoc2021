#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>

int main() {
    std::ifstream ifs("inputs/01a.txt");
    if (!ifs.is_open()) {
        std::cerr << "file not found" << std::endl;
        return 1;
    }

    std::vector<int> depths(std::istream_iterator<int>{ifs},
                            std::istream_iterator<int>{});

    auto incs = 0;
    for (std::vector<int>::size_type i = 3; i < depths.size(); i++) {
        if (depths[i] > depths[i-3]) {
            incs++;
        }
    }
    
    std::cout << incs << std::endl;
    return 0;
}
