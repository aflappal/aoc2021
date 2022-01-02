#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <ranges>
#include "util.h"

using std::cout, std::cerr;

using Rules = std::map<std::string, std::string>;

struct Input {
    std::string initial;
    Rules rules;
};

auto parse() {
    auto ifs = au::get_ifstream("inputs/14.txt");
    Input input;
    std::string line;
    std::getline(ifs, input.initial);
    std::getline(ifs, line); // skip empty line

    while (std::getline(ifs, line)) {
        std::istringstream iss{line};
        // XXX result could be char
        std::string pair, result;
        iss >> pair;
        // skip " -> "
        iss.ignore(10, ' '); iss.ignore(10, ' ');
        iss >> result;
        iss.ignore(1, '\n');
        input.rules[pair] = result;
    }
    return input;
}

const auto input = parse();

auto augment(const std::string& current, const Rules& rules) {
    std::string next = current.substr(0, 1);
    for (std::size_t i = 1; i < current.size(); ++i) {
        auto pair = current.substr(i-1, 2);
        next += rules.at(pair) + current[i];
    }
    return next;
}

void solve_a() {
    const auto& [initial, rules] = input;
    auto current = initial;
    for (auto i = 0; i < 10; ++i) {
        current = augment(current, rules);
    }

    auto occurrences = std::map<char, int>();
    for (const auto& c : current)
        ++occurrences[c];
    auto [min, max] = std::ranges::minmax(occurrences | std::views::values);
    cout << max - min << '\n';
}

void solve_b() {
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
