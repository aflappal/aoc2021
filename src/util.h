#pragma once

#include <iosfwd>
#include <fstream>
#include <iterator>
#include <vector>
#include <functional>

namespace au {

using solve_func = std::function<void ()>;
void solve_runner(solve_func solve_a, solve_func solve_b) {
    std::cout << "a:\n";
    solve_a();
    std::cout << "-------------------------\n"
              << "b:\n";
    solve_b();
}

std::ifstream get_ifstream(std::string path) {
    std::ifstream ifs(path);
    if (!ifs) {
        std::cerr << "file not found\n";
        std::exit(1);
    }

    return ifs;
}

// Reads file into vector of T. Assumes T has a defined operator>>. T can for
// example be a pair of (string, int) which would need a >> that reads the
// stream into T properly.
template <typename T>
std::vector<T> get_input_vector_from_file(std::string path) {
    auto ifs = get_ifstream(path);

    return std::vector<T>(std::istream_iterator<T>{ifs},
                          std::istream_iterator<T>{});
}

} // namespace au
