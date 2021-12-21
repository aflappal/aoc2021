#include <iostream>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>
#include <numeric>
#include "util.h"

using std::cout, std::cerr;

struct Point {
    int x;
    int y;
    int val;
};

using grid_type = std::vector<std::vector<Point>>;
using window_type = std::array<Point, 4>;

auto parse() {
    auto ifs = au::get_ifstream("inputs/09.txt");
    grid_type grid;
    std::string line;
    for (std::size_t y = 0; std::getline(ifs, line); ++y) {
        std::vector<Point> line_nums;
        for (std::size_t x = 0; x < line.size(); ++x) {
            line_nums.push_back({(int)x, (int)y, line[x]-'0'});
        }
        grid.push_back(line_nums);
    }
    return grid;
}

const auto input = parse();

// Window of dummy points; just using them for the coordinates
auto basic_window(const Point& p) {
    return window_type {{
                         {p.x, p.y-1, -1},
        {p.x-1, p.y, -1},     /* p */     {p.x+1, p.y, -1},
                         {p.x, p.y+1, -1}
    }};
}

auto limit(window_type&& window, const Point& se_corner) {
    auto within_grid = [&](const Point& p) {
        return 0 <= p.x && p.x <= se_corner.x && 0 <= p.y && p.y <= se_corner.y;
    };
    return window | std::views::filter(within_grid);
}

auto elements(const grid_type& grid, auto window) {
    return window
        | std::views::transform([&](const Point& p) { return grid[p.y][p.x]; });
}

void solve_a() {
    auto grid = input;
    auto is_low = [&](const Point& p) {
        auto window = limit(basic_window(p), grid.back().back());
        auto higher = [&](const Point& other) { return other.val > p.val; };
        return std::ranges::all_of(elements(grid, window), higher);
    };
    auto low_points = grid | std::views::join | std::views::filter(is_low);
    auto accu_func = [&](int accu, const Point& p) {
        return accu + p.val + 1;
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
