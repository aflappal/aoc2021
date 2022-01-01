#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>
#include <ranges>
#include "util.h"

using std::cout, std::cerr;

struct Point {
    int x;
    int y;
};

// Dimensions are 2*first_fold + 1
using Grid = std::array<std::array<bool, 1311>, 895>;

struct Fold {
    char along;
    int pos;
};

struct Input {
    Grid grid;
    std::vector<Fold> folds;
};

auto parse() {
    auto ifs = au::get_ifstream("inputs/13.txt");
    Input input;
    std::string line;
    while (std::getline(ifs, line)) {
        if (line == "")
            break;
        std::istringstream iss{line};
        std::string xs, ys;
        std::getline(iss, xs, ',');
        std::getline(iss, ys);
        input.grid[std::stoi(ys)][std::stoi(xs)] = true;
    }
    while (std::getline(ifs, line)) {
        std::istringstream iss{line};
        // Skip "fold along "
        iss.ignore(10, ' '); iss.ignore(10, ' ');
        std::string along, pos;
        std::getline(iss, along, '=');
        std::getline(iss, pos);
        // XXX can't emplace back for some reason
        input.folds.push_back({along[0], std::stoi(pos)});
    }
    return input;
}

const auto input = parse();

auto count_visible(const Grid& grid, const Point& se_corner) {
    auto limited = grid
                 | std::views::take(se_corner.y)
                 | std::views::transform(
                         [&](const auto& arr) { return arr | std::views::take(se_corner.x); })
                 | std::views::join;
    const auto is_visible = [&grid](bool v) { return v; };
    return std::ranges::count_if(limited, is_visible);
}

// All folds are in the middle of the row/column
auto do_fold(Grid& grid, const Fold& fold, const Point& se_corner) {
    if (fold.along == 'x') {
        for (auto y = 0; y < se_corner.y; ++y) {
            for (auto x = fold.pos + 1; x < se_corner.x; ++x) {
                grid[y][se_corner.x - 1 - x] |= grid[y][x];
            }
        }
        return Point{fold.pos, se_corner.y};
    } else {
        for (auto y = fold.pos + 1; y < se_corner.y; ++y) {
            for (auto x = 0; x < se_corner.x; ++x) {
                grid[se_corner.y - 1 - y][x] |= grid[y][x];
            }
        }
        return Point{se_corner.x, fold.pos};
    }
}

void solve_a() {
    auto [grid, folds] = input;
    auto se_corner = Point{(int)grid.back().size(), (int)grid.size()};
    se_corner = do_fold(grid, folds[0], se_corner);
    cout << count_visible(grid, se_corner) << '\n';
}

void print(const Grid& grid, const Point& se_corner) {
    for (auto y = 0; y < se_corner.y; ++y) {
        for (auto x = 0; x < se_corner.x; ++x) {
            cout << (grid[y][x] ? '#' : '.');
        }
        cout << '\n';
    }
}

void solve_b() {
    auto [grid, folds] = input;
    auto se_corner = Point{(int)grid.back().size(), (int)grid.size()};
    for (const auto& fold : folds) {
        se_corner = do_fold(grid, fold, se_corner);
    }
    print(grid, se_corner);
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
