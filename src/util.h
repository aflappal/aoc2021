#pragma once

#include <iosfwd>
#include <fstream>
#include <iterator>
#include <vector>
#include <functional>

namespace au {

using solve_func_type = std::function<void ()>;
void solve_runner(solve_func_type solve_a, solve_func_type solve_b) {
    std::cout << "a:" << std::endl;
    solve_a();
    std::cout << "-------------------------" << std::endl
              << "b:" << std::endl;
    solve_b();
}

// XXX: memoize somehow?
// XXX: ok to return new ifstream? Or should it be a pointer or something?
std::ifstream get_ifstream(std::string path) {
    std::ifstream ifs(path);
    if (!ifs) {
        std::cerr << "file not found" << std::endl;
        std::exit(1);
    }

    return ifs;
}

// XXX: memoize somehow?
// XXX: ok to return new vector? Or should it be a pointer or something?
// Reads file into vector of T line by line. Assumes T has defined operator >>.
// T can for example be a pair of (string, int) which would need a >> that
// reads the stream into T properly.
template <typename T>
std::vector<T> get_input_vector_from_file(std::string path) {
    auto ifs = get_ifstream(path);

    return std::vector<T>(std::istream_iterator<T>{ifs},
                          std::istream_iterator<T>{});
}

} // namespace au
