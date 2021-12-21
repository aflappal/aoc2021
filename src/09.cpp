#include <iostream>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>
#include <numeric>
#include "util.h"

using std::cout, std::cerr;

// XXX include val into point? Wouldn't work too well with the static windowing
struct Point {
    int x;
    int y;
};

using grid_type = std::vector<std::vector<int>>;
using window_type = std::array<Point, 4>;

auto at(const grid_type& grid, const Point& p) {
    return grid[p.y][p.x];
}

auto parse() {
    auto ifs = au::get_ifstream("inputs/09.txt");
    grid_type grid;
    std::string line;
    while (std::getline(ifs, line)) {
        std::vector<int> line_nums;
        for (auto c : line) {
            line_nums.push_back(c - '0');
        }
        grid.push_back(line_nums);
    }
    return grid;
}

auto make_points(const grid_type& grid) {
    std::vector<Point> points;
    for (std::size_t y = 0; y < grid.size(); ++y) {
        auto row = grid[y];
        for (std::size_t x = 0; x < row.size(); ++x) {
            points.push_back(Point{(int)x, (int)y});
        }
    }
    return points;
}

const auto input_vals = parse();
const auto input_points = make_points(input_vals);

auto neighbours(const Point& p) {
    return window_type {{
                     {p.x, p.y-1},
        {p.x-1, p.y},   /* p */   {p.x+1, p.y},
                     {p.x, p.y+1}
    }};
}

auto limit(window_type&& window, const Point& se_corner) {
    auto within_grid = [&](Point& p) {
        return 0 <= p.x && p.x <= se_corner.x && 0 <= p.y && p.y <= se_corner.y;
    };
    return window | std::views::filter(within_grid);
}

auto elements(const grid_type& grid, auto window) {
    return window
        | std::views::transform([&](Point& p) { return at(grid, p); });
}

void solve_a() {
    auto grid = input_vals;
    auto points = input_points;
    auto is_low = [&](const Point& p) {
        auto window = limit(neighbours(p), points.back());
        auto higher = [&](int val) { return val > at(grid, p); };
        return std::ranges::all_of(elements(grid, window), higher);
    };
    auto low_points = points | std::views::filter(is_low);
    auto accu_func = [&](int accu, const Point& p) {
        return accu + at(grid, p) + 1;
    };
    auto sum = std::accumulate(low_points.begin(), low_points.end(), 0, accu_func);
    cout << sum << '\n';
}

void solve_b() {
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
