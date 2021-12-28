#include <iostream>
#include <sstream>
#include <algorithm>
#include <array>
#include <vector>
#include <functional>
#include "util.h"

using std::cout, std::cerr;

struct Point {
    Point() {}
    Point(int x_, int y_) : x(x_), y(y_) {}
    int x;
    int y;
};

struct Line {
    std::vector<Point> all_points() const {
        std::vector<Point> points;
        auto xinc = vertical() ? 0 : (start.x < end.x ? 1 : -1);
        auto yinc = horizontal() ? 0 : (start.y < end.y ? 1 : -1);
        for (Point p{start};
             p.x != end.x + xinc || p.y != end.y + yinc;
             p.x += xinc, p.y += yinc) {
            points.push_back(Point(p.x, p.y));
        }
        return points;
    }
    bool horizontal() const {
        return start.y == end.y;
    }
    bool vertical() const {
        return start.x == end.x;
    }

    Point start;
    Point end;
};

std::ostream& operator<<(std::ostream& stream, const Point& out) {
    return stream << out.x << ' ' << out.y;
}

std::istream& operator>>(std::istream& stream, Point& in) {
    std::string coords;
    stream >> coords;
    std::ranges::replace(coords, ',', ' ');
    std::istringstream iss(coords);
    iss >> in.x >> in.y;
    return stream;
}

std::istream& operator>>(std::istream& stream, Line& in) {
    std::string sep;
    return stream >> in.start >> sep >> in.end;
}

const auto lines = au::get_input_vector_from_file<Line>("inputs/05.txt");

void count_overlaps(std::function<bool(const Line&)> line_filter =
        [](const Line&) { return true; }) {
    std::array<std::array<int, 1000>, 1000> grid{0};

    for (const auto& points : lines
                            | std::views::filter(line_filter)
                            | std::views::transform(
                                [](const Line& line) {
                                    return line.all_points();
                                })) {
        for (const auto& point : points) {
            grid[point.x][point.y]++;
        }
    }
    // XXX doesn't work for some reason. Join doesn't join transform of vectors
    //for (const auto& point : lines
    //                       | std::views::filter(horiz_or_vert)
    //                       | std::views::transform(
    //                           [](const Line& line) {
    //                               return line.all_points();
    //                           })
    //                       | std::views::join) {
    //    grid[point.x][point.y]++;
    //}

    auto more_than_one = [](int num) { return num > 1; };
    auto overlaps = std::ranges::count_if(grid | std::views::join, more_than_one);
    cout << overlaps << '\n';
}

void solve_a() {
    auto horiz_or_vert = [](const Line& line) {
        return line.horizontal() || line.vertical();
    };
    count_overlaps(horiz_or_vert);
}

void solve_b() {
    count_overlaps();
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
