#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include "util.h"

using std::cout, std::cerr;

auto parse() {
    auto ifs = au::get_ifstream("inputs/07.txt");
    std::vector<int> nums;
    std::string num;
    while (std::getline(ifs, num, ',')) {
        nums.push_back(std::stoi(num));
    }
    return nums;
}

const auto positions = parse();

void solve_a() {
    auto vals = positions;

    // calculate median
    auto m = vals.begin() + vals.size()/2;
    std::nth_element(vals.begin(), m, vals.end());
    int median;
    if ((vals.size() % 2) == 0) {
        auto k = std::max_element(vals.begin(), m);
        median = (*m + *k) / 2;
    } else {
        median = *m;
    }

    // calculate sum of distances to median
    auto dist_median = [=](int v) { return std::abs(v - median); };
    std::transform(vals.begin(), vals.end(), vals.begin(), dist_median);
    cout << std::accumulate(vals.begin(), vals.end(), 0) << '\n';
}

void solve_b() {
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
    //for (auto num : positions) cout << num << ' ';
    //cout << '\n';
}
