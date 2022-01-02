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

auto augment(const std::map<std::string, long> current, const Rules& rules) {
    std::map<std::string, long> next;
    for (const auto& [pair, n] : current) {
        const auto product = rules.at(pair);
        const std::string first = pair[0] + product;
        const std::string second = product + pair[1];
        next[first] += n;
        next[second] += n;
    }
    return next;
}

auto make_pairs(const std::string& s) {
    std::map<std::string, long> pairs;
    for (std::size_t i = 1; i < s.size(); ++i) {
        auto pair = s.substr(i-1, 2);
        ++pairs[pair];
    }
    return pairs;
}

auto answer_after_iters(int iters) {
    const auto& [initial, rules] = input;
    auto pairs = make_pairs(initial);
    for (auto i = 0; i < iters; ++i) {
        pairs = augment(pairs, rules);
    }

    auto occurrences = std::map<char, long>();
    for (const auto& [pair, n] : pairs) {
        occurrences[pair[1]] += n;
    }
    // Before only the second char of each pair was added because the first is
    // the second of some other pair. Except for the first character; add it
    // explicitly here.
    // XXX is this correct..?
    ++occurrences[initial[0]];
    auto [min, max] = std::ranges::minmax(occurrences | std::views::values);
    return max-min;
}

void solve_a() {
    cout << answer_after_iters(10) << std::endl;
}

void solve_b() {
    cout << answer_after_iters(40) << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
