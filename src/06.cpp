#include <iostream>
#include <sstream>
#include <array>
#include <algorithm>
#include <numeric>
#include "util.h"

using std::cout, std::cerr;

const auto N = 9;

class Counts {
public:
    Counts(const std::array<unsigned long, N>& initial)
        : _start_idx{0}, _days_elapsed{0}, _counts{initial}
    {}
    auto size() const { return _counts.size(); }
    auto operator[](std::size_t i) const {
        return _counts[(_start_idx + i) % size()];
    }
    auto& operator[](std::size_t i) {
        return _counts[(_start_idx + i) % size()];
    }
    void simulate() {
        // 0s spawn new 8s and reset to 6s. At this point the next 6s are still
        // 7s.
        (*this)[7] += (*this)[0];
        _start_idx = (_start_idx + 1) % size();
        ++_days_elapsed;
    }
    void simulate_until(int day) {
        while (_days_elapsed < day) {
            simulate();
        }
    }
    unsigned long count() const {
        return std::accumulate(_counts.begin(), _counts.end(), 0ul);
    }

private:
    std::size_t _start_idx;
    int _days_elapsed;
    std::array<unsigned long, N> _counts;
};

auto make_initial_counts(std::istream& is) {
    std::string nums;
    is >> nums;
    std::ranges::replace(nums, ',', ' ');

    std::istringstream iss{nums};
    int num;
    std::array<unsigned long, N> arr = {0};
    while (iss >> num) {
        arr[num]++;
    }

    return arr;
}

auto ifs = au::get_ifstream("inputs/06.txt");
const auto initial_counts = make_initial_counts(ifs);

void solve_a() {
    auto counts = Counts(initial_counts);
    counts.simulate_until(80);
    cout << counts.count() << '\n';
}

void solve_b() {
    auto counts = Counts(initial_counts);
    counts.simulate_until(256);
    cout << counts.count() << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
