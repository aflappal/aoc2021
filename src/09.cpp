#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include "util.h"

using std::cout, std::cerr;

auto parse() {
    //auto ifs = au::get_ifstream("inputs/test2_09.txt");
    auto ifs = au::get_ifstream("inputs/09.txt");
    std::vector<std::vector<int>> grid;
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

const auto input = parse();

struct Point {
    int x;
    int y;
};

// Get window around o(rigo) limited to the rect defined by corners {0, 0} and
// se_corner, inclusive.
auto neighbours(std::vector<std::vector<int>>& grid, Point& o) {
    Point window[] = {              {o.x, o.y-1},
                      {o.x-1, o.y},                 {o.x+1, o.y},
                                    {o.x, o.y+1}
    };
    Point se_corner = {static_cast<int>(grid[0].size()-1), static_cast<int>(grid.size()-1)};
    auto within_grid = [&](Point& p) {
        return 0 <= p.x && p.x <= se_corner.x && 0 <= p.y && p.y <= se_corner.y;
    };
    auto neighbour_view = window | std::views::filter(within_grid);
    std::vector<int> neighbours;
    std::ranges::transform(neighbour_view, std::back_inserter(neighbours),
            [&](Point& p) { return grid[p.y][p.x]; });
    return neighbours;
}

void solve_a() {
    auto grid = input;
    auto is_low = [&](Point&& p) {
        auto higher = [&](int val) { return val > grid[p.y][p.x]; };
        return std::ranges::all_of(neighbours(grid, p), higher);
    };
    int sum = 0;
    for (std::size_t y = 0; y < grid.size(); ++y) {
        auto row = grid[y];
        for (std::size_t x = 0; x < row.size(); ++x) {
            if (is_low({(int)x, (int)y})) {
                sum += row[x] + 1;
                cout << "low " << x << ' ' << y << ", add " << row[x] + 1 << ", cumsum " << sum << '\n';
            }
        }
    }
    cout << sum << '\n';
}

void solve_b() {
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
