#include <iostream>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <vector>
#include <limits>
#include "util.h"

using std::cout, std::cerr;

auto parse() {
    auto ifs = au::get_ifstream("inputs/07.txt");
    //std::istringstream ifs{"16,1,2,0,4,2,7,1,2,14"};
    //std::istringstream ifs{"1,2,5,7"};
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
auto distances_to(std::vector<int> vals, int to, std::string part) {
    auto dist_a = [=](int v) { return std::abs(v - to); };
    auto dist_b = [&](int v) { auto n = dist_a(v); return n*(n+1)/2; };
    if (part == "a")
        std::transform(vals.begin(), vals.end(), vals.begin(), dist_a);
    else
        std::transform(vals.begin(), vals.end(), vals.begin(), dist_b);
    return std::accumulate(vals.begin(), vals.end(), 0);
}

void solve_a() {
    auto vals = positions;
    // Calculate middlest number, meaning median when vals.size() is odd or
    // either of the middle numbers (so median = avg of the middle numbers is
    // not needed here; the distances to either will be the same even if the
    // numbers differ) when vals.size() is even.
    auto middle = middlest(vals);
    cout << "middle: " << middle << '\n';
    // calculate sum of distances to middle
    cout << "a: " << distances_to(vals, middle, "a") << '\n';

}

void solve_b() {
    auto vals = positions;
    auto best_candidate = -1;
    auto best_dist = std::numeric_limits<int>::max();
    for (auto candidate = *std::ranges::min_element(vals);
         candidate <= *std::ranges::max_element(vals);
         ++candidate) {
        auto dist = distances_to(vals, candidate, "b");
        if (dist < best_dist) {
            best_candidate = candidate;
            best_dist = dist;
        }
    }
    cout << "new middle: " << best_candidate << '\n';
    cout << "b: " << best_dist << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
