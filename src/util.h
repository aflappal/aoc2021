#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <functional>
#include <utility>
#include <chrono>
#include <ratio>
#include <type_traits>
#include <ranges>

namespace au {

template<typename F, typename... Args,
    typename F_R = std::invoke_result_t<F, Args...>,
    typename R = std::conditional_t<std::is_void_v<F_R>, int, F_R>>
R timed(F func, Args&&... args) {
    using std::chrono::steady_clock;
    using std::chrono::duration;

    auto t1 = steady_clock::now();
    // XXX doesn't work for references I guess?
    R result;
    // If F's real return type is void then return 0 instead (we can't declare
    // a void variable..)
    if constexpr (std::is_void_v<F_R>) {
        result = 0;
        func(std::forward<Args>(args)...);
    } else {
        result = func(std::forward<Args>(args)...);
    }
    auto t2 = steady_clock::now();
    std::cout << "\nTook " << duration<double, std::milli>(t2-t1).count() << " ms.\n";
    return result;
}

using solve_func = std::function<void ()>;
void solve_runner(solve_func solve_a, solve_func solve_b) {
    std::cout << "a:\n";
    timed(solve_a);
    std::cout << "-------------------------\n"
              << "b:\n";
    timed(solve_b);
}

std::ifstream get_ifstream(std::string path) {
    std::ifstream ifs(path);
    if (!ifs) {
        std::cerr << "file not found\n";
        std::exit(1);
    }

    return ifs;
}

template <typename T>
std::vector<T> get_vector_from_stream(std::istream& is) {
    return std::vector<T>(std::istream_iterator<T>{is},
                          std::istream_iterator<T>{});
}

// Reads file into vector of T. Assumes T has a defined operator>>. T can for
// example be a pair of (string, int) which would need a >> that reads the
// stream into T properly.
template <typename T>
std::vector<T> get_input_vector_from_file(std::string path) {
    auto ifs = get_ifstream(path);
    return get_vector_from_stream<T>(ifs);
}

// func = [](auto) { return ..; }, so with "no" arguments
auto generate_n(auto& func, auto count) {
    return std::views::iota(0, count) | std::views::transform(func);
}

auto split_line(std::istringstream& iss, const char delim) {
    std::string first, second;
    std::getline(iss, first, delim);
    std::getline(iss, second);
    return std::pair{first, second};
}

auto split_line(std::string& s, const char delim) {
    std::istringstream iss{s};
    return split_line(iss, delim);
}

} // namespace au
