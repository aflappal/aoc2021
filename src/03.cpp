#include <iostream>
#include <bitset>
#include <array>
#include "util.h"

using std::cout, std::cerr;

const auto bitwidth = 12;
const auto vals = au::get_input_vector_from_file<std::bitset<bitwidth>>("inputs/03.txt");

auto most_common_bits(const std::vector<std::bitset<bitwidth>>& from) {
    std::array<std::size_t, bitwidth> ones{0};
    for (const auto& val : from) {
        for (std::size_t bitpos = 0; bitpos < val.size(); bitpos++) {
            ones[bitpos] += val[bitpos];
        }
    }

    std::bitset<bitwidth> most_common{0};
    const auto cutoff = (from.size() + 1) / 2;
    for (std::size_t bitpos = 0; bitpos < ones.size(); bitpos++) {
        if (ones[bitpos] >= cutoff) {
            most_common[bitpos] = 1;
        }
    }

    return most_common;
}

auto val_for_commonality(std::vector<std::bitset<bitwidth>> from, bool most_common) {
    std::size_t bitpos = bitwidth - 1;
    while (from.size() > 1) {
        // XXX: useless to get all the most common bits when we only need it
        // for one bitpos, but did it this way due to a misunderstanding of the
        // requirements originally :|
        auto pattern = most_common_bits(from);
        if (!most_common) pattern = ~pattern;
            from.erase(std::remove_if(from.begin(), from.end(),
                [&](const std::bitset<bitwidth>& set) {
                    return set[bitpos] != pattern[bitpos];
                }), from.end());
        bitpos--;
    }
    return from[0];
}

void solve_a() {
    auto gamma = most_common_bits(vals);
    auto epsilon = ~gamma;

    cout << "gamma: " << gamma << ", epsilon: " << epsilon
         << ", gamma * epsilon = " << gamma.to_ulong() * epsilon.to_ulong()
         << '\n';
}

void solve_b() {
    auto oxy = val_for_commonality(vals, true);
    auto co2 = val_for_commonality(vals, false);

    cout << "oxy: " << oxy << ", co2: " << co2
         << ", oxy * co2 = " << oxy.to_ulong() * co2.to_ulong()
         << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
