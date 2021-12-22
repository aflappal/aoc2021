#include <iostream>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <functional>
#include "util.h"

using std::cout, std::cerr;

struct Point {
    int x;
    int y;
};

struct Cell {
    Point coords;
    int val;
};

using grid_type = std::vector<std::vector<Cell>>;
using window_type = std::array<Point, 4>;

auto parse() {
    auto ifs = au::get_ifstream("inputs/09.txt");
    grid_type grid;
    std::string line;
    for (std::size_t y = 0; std::getline(ifs, line); ++y) {
        std::vector<Cell> line_nums;
        for (std::size_t x = 0; x < line.size(); ++x) {
            line_nums.push_back({(int)x, (int)y, line[x]-'0'});
        }
        grid.push_back(line_nums);
    }
    return grid;
}

const auto input = parse();

auto basic_window(const Point& p) {
    return window_type {{
                     {p.x, p.y-1},
        {p.x-1, p.y},   /* p */   {p.x+1, p.y},
                     {p.x, p.y+1}
    }};
}

auto limit(window_type&& window, const Point& se_corner) {
    auto within_grid = [&se_corner](const Point& p) {
        return 0 <= p.x && p.x <= se_corner.x && 0 <= p.y && p.y <= se_corner.y;
    };
    return window | std::views::filter(within_grid);
}

auto elements(grid_type& grid, auto window) {
    return window
        | std::views::transform([&grid](const Point& p) { return grid[p.y][p.x]; });
}

void solve_a() {
    auto grid = input;
    auto is_low = [&grid](const Cell& c) {
        auto window = limit(basic_window(c.coords), grid.back().back().coords);
        auto higher = [&c](const Cell& other) { return other.val > c.val; };
        return std::ranges::all_of(elements(grid, window), higher);
    };
    auto low_points = grid | std::views::join | std::views::filter(is_low);
    auto accu_func = [](int accu, const Cell& c) {
        return accu + c.val + 1;
    };
    auto sum = std::accumulate(low_points.begin(), low_points.end(), 0, accu_func);
    cout << sum << '\n';
}

bool visited(const Cell& cell) {
    return cell.val < 0 || cell.val == 9;
}

void visit(Point& point, grid_type& grid) {
    // lazy way to mark cell as visited
    grid[point.y][point.x].val = -1;
}

bool dfs(Cell& cell, grid_type& grid, std::vector<int>& labels) {
    if (visited(cell))
        return false;
    visit(cell.coords, grid);
    // just incrementing number. Don't bother labeling the cell itself since
    // that doesn't matter here
    labels.back()++;
    auto window = limit(basic_window(cell.coords), grid.back().back().coords);
    for (auto&& c : elements(grid, window)) {
        dfs(c, grid, labels);
    }
    return true;
}

void solve_b() {
    auto grid = input;
    // each label value corresponds to the number of cells belonging to a
    // certain connected component
    std::vector<int> labels;
    labels.push_back(0);
    for (auto& cell : grid | std::views::join) {
        if (dfs(cell, grid, labels)) {
            // new component was just added, so add new label
            labels.push_back(0);
        };
    }

    // partial sort so that greatest three elements are at the front
    std::ranges::partial_sort(labels, labels.begin()+3, std::greater{});
    cout << std::accumulate(labels.begin(), labels.begin()+3,
                            1, std::multiplies<int>{});
    cout << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
