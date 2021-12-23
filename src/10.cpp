#include <iostream>
#include <vector>
#include <cassert>
#include "util.h"

using std::cout, std::cerr;

const auto input = au::get_input_vector_from_file<std::string>("inputs/10.txt");

auto error_value_for_char(char c) {
    switch (c) {
    case ')': return 3;
    case '}': return 1197;
    case ']': return 57;
    case '>': return 25137;
    }
    return -999999;
}

auto closing_paren(char opening) {
    switch (opening) {
    case '(': return ')';
    case '{': return '}';
    case '[': return ']';
    case '<': return '>';
    }
    return '\0';
}

auto is_opening_paren(char c) {
    return c == '(' || c == '{' || c == '[' || c == '<';
}

void solve_a() {
    auto error = 0;
    for (auto& line : input) {
        std::vector<char> stack;
        stack.reserve(16);
        for (auto c : line) {
            if (is_opening_paren(c)) {
                stack.push_back(c);
            } else {
                assert(stack.size() > 0);
                char opening = stack.back();
                stack.pop_back();
                // corrupted?
                if (c != closing_paren(opening)) {
                    error += error_value_for_char(c);
                    break;
                }
            }
        }
    }

    cout << error << '\n';
}

void solve_b() {
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
