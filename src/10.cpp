#include <iostream>
#include <vector>
#include <algorithm>
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

auto completion_for_opening(char c) {
    switch (c) {
    case '(': return 1;
    case '[': return 2;
    case '{': return 3;
    case '<': return 4;
    }
    return -999999;
}

void solve() {
    auto error = 0;
    std::vector<long> completions;
    for (auto& line : input) {
        std::vector<char> stack;
        stack.reserve(16);
        bool corrupted = false;
        for (auto c : line) {
            if (is_opening_paren(c)) {
                stack.push_back(c);
            } else {
                assert(stack.size() > 0);
                char opening = stack.back();
                stack.pop_back();
                // corrupted?
                if (c != closing_paren(opening)) {
                    corrupted = true;
                    error += error_value_for_char(c);
                    break;
                }
            }
        }

        // incomplete?
        if (!corrupted && stack.size() > 0) {
            auto completion = 0l;
            while (stack.size() > 0) {
                auto c = stack.back();
                stack.pop_back();
                // scores were specified for closing parens but just doing it
                // directly for opening parens here
                completion = 5 * completion + completion_for_opening(c);
            }
            completions.push_back(completion);
        }
    }

    cout << "a: " << error << '\n';

    auto middle = completions.begin() + completions.size() / 2;
    std::nth_element(completions.begin(), middle, completions.end());
    cout << "b: " << *middle << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    solve();
}
