#include <iostream>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <vector>
#include <ranges>
#include <cmath>
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

auto distances_to(std::vector<int>& vals, int to, std::string part) {
    auto dist = [&](int v) -> int {
        auto basic_dist = std::abs(v - to);
        // Silly way to select but can't easily select between multiple lambdas
        // since they have unique types
        if (part == "a")
            return basic_dist;
        else
            return basic_dist*(basic_dist+1)/2;
    };
    auto deltas = vals | std::views::transform(dist);
    return std::accumulate(deltas.begin(), deltas.end(), 0);
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
    cout << distances_to(vals, middle, "a") << '\n';
}

void solve_b() {
    auto vals = positions;
    // The best position is either ceil(mean) or floor(mean)
    // (see https://www.reddit.com/r/adventofcode/comments/rar7ty/2021_day_7_solutions/hnv4tds/
    // for potentially fancier way of determining which is better)
    auto mean = static_cast<float>(std::accumulate(vals.begin(), vals.end(), 0))
        / vals.size();
    cout << "new position either " << std::floor(mean) << " or "
         << std::ceil(mean) << '\n';
    auto best = std::min(distances_to(vals, std::floor(mean), "b"),
                         distances_to(vals, std::ceil(mean), "b"));
    cout << best << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
