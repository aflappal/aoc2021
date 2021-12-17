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

// Reorders enough elements of vals to find the middle
auto middlest(std::vector<int>& vals) {
    auto m = vals.begin() + vals.size()/2;
    std::nth_element(vals.begin(), m, vals.end());
    return *m;
}

// vals copied because we want to use the untransformed vals later
auto distances_to(std::vector<int> vals, int to) {
    auto dist = [=](int v) { return std::abs(v - to); };
    std::transform(vals.begin(), vals.end(), vals.begin(), dist);
    return std::accumulate(vals.begin(), vals.end(), 0);
}

void solve_a() {
    auto vals = positions;

    // Calculate middlest number, meaning median when vals.size() is odd or
    // either of the middle numbers (so median = avg of the middle numbers is
    // not needed here; the distances to either will be the same even if the
    // numbers differ) when vals.size() is even.
    auto middle = middlest(vals);

    // calculate sum of distances to middle
    cout << distances_to(vals, middle) << '\n';
}

void solve_b() {
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
    //for (auto num : positions) cout << num << ' ';
    //cout << '\n';
}
