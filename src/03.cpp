#include <iostream>
#include <bitset>
#include <array>
#include "util.h"

using std::cout, std::cerr;

const auto vals = au::get_input_vector_from_file<std::bitset<12>>("inputs/03.txt");

void solve_a() {
    std::array<std::size_t, 12> ones{0};
    for (const auto& val : vals) {
        for (std::size_t bitpos = 0; bitpos < val.size(); bitpos++) {
            ones[bitpos] += val[bitpos];
        }
    }

    std::bitset<12> gamma{0};
    const auto cutoff = vals.size() / 2;
    for (std::size_t bitpos = 0; bitpos < ones.size(); bitpos++) {
        if (ones[bitpos] > cutoff) {
            gamma[bitpos] = 1;
        }
    }
    auto epsilon = ~gamma;

    cout << "gamma: " << gamma << ", epsilon: " << epsilon
         << ", gamma * epsilon = " << gamma.to_ulong() * epsilon.to_ulong()
         << '\n';
}

void solve_b() {
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
