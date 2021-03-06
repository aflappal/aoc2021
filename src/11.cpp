#include <iostream>
#include <vector>
#include <array>
#include <ranges>
#include <numeric>
#include "util.h"

using std::cout, std::cerr;

struct Point {
    int x;
    int y;
};

struct Cell {
    Point coords;
    int energy;
    bool flashed;
};

using grid_type = std::vector<std::vector<Cell>>;
using window_type = std::array<Point, 8>;

auto parse() {
    auto ifs = au::get_ifstream("inputs/11.txt");
    grid_type grid;
    std::string line;
    for (std::size_t y = 0; std::getline(ifs, line); ++y) {
        std::vector<Cell> line_nums;
        for (std::size_t x = 0; x < line.size(); ++x) {
            line_nums.push_back({(int)x, (int)y, line[x]-'0', false});
        }
        grid.push_back(line_nums);
    }
    return grid;
}

const auto input = parse();

auto basic_window(const Point& p) {
    return window_type {{
        {p.x-1, p.y-1}, {p.x, p.y-1}, {p.x+1, p.y-1},
        {p.x-1, p.y},     /* p */     {p.x+1, p.y},
        {p.x-1, p.y+1}, {p.x, p.y+1}, {p.x+1, p.y+1}
    }};
}

auto limit(const window_type& window, const Point& se_corner) {
    auto within_grid = [&se_corner](const Point& p) {
        return 0 <= p.x && p.x <= se_corner.x && 0 <= p.y && p.y <= se_corner.y;
    };
    return window | std::views::filter(within_grid);
}

auto elements(grid_type& grid, auto window) {
    auto point_to_cell_ref = [&grid](const Point& p) -> Cell& { return grid[p.y][p.x]; };
    return window | std::views::transform(point_to_cell_ref);
}

void increment(auto cells) {
    for (auto& cell : cells)
        ++cell.energy;
}

auto reset_flashed(grid_type& grid) {
    auto flashed = [](Cell& cell) { return cell.flashed; };
    auto count = 0;
    for (auto& cell : grid | std::views::join | std::views::filter(flashed)) {
        cell.flashed = false;
        cell.energy = 0;
        ++count;
    }
    return count;
}

auto do_flashes(grid_type& grid) {
    auto flashes = 0;
    auto se_corner = grid.back().back().coords;
    auto must_flash = [](Cell& cell) { return cell.energy > 9 && !cell.flashed; };
    for (auto& cell : grid | std::views::join | std::views::filter(must_flash)) {
        cell.flashed = true;
        ++flashes;
        auto basic = basic_window(cell.coords);
        increment(elements(grid, limit(basic, se_corner)));
    }
    return flashes;
}

auto do_step(grid_type& grid) {
    increment(grid | std::views::join);
    while (do_flashes(grid) > 0)
        ;
    auto flashes = reset_flashed(grid);
    return flashes;
}

void solve_a() {
    auto grid = input;
    auto step_func = [&grid](auto) { return do_step(grid); };
    auto step_flashes = au::generate_n(step_func, 100);
    auto flashes = std::accumulate(step_flashes.begin(), step_flashes.end(), 0);
    cout << flashes << '\n';
}

void solve_b() {
    auto grid = input;
    const auto size = grid.size() * grid.front().size();
    auto step = 0;
    // all flashed at the same time when num flashed == size
    while (do_step(grid) != (int)size)
        ++step;
    cout << step+1 << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
