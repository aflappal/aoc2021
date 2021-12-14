#include <iostream>
#include <sstream>
#include <array>
#include <algorithm>
#include <numeric>
#include "util.h"

using std::cout, std::cerr;

const auto N = 9;
using inttype = unsigned long;
using container = std::array<inttype, N>;

class Counts {
public:
    Counts(const container& initial)
        : _start_idx{0}, _days_elapsed{0}, _counts{initial}
    {}
    void simulate() {
        // 0s spawn new 8s and reset to 6s. At this point the next 6s are still
        // 7s.
        _counts[idx(7)] += _counts[idx(0)];
        _start_idx = (_start_idx + 1) % _counts.size();
        ++_days_elapsed;
    }
    void simulate_until(int day) {
        while (_days_elapsed < day) {
            simulate();
        }
    }
    inttype count() const {
        return std::accumulate(_counts.begin(), _counts.end(), inttype{0});
    }

private:
    std::size_t idx(std::size_t i) const {
        return (_start_idx + i) % _counts.size();
    }
    std::size_t _start_idx;
    int _days_elapsed;
    container _counts;
};

auto make_initial_counts(std::istream& is) {
    std::string nums;
    is >> nums;
    std::ranges::replace(nums, ',', ' ');

    std::istringstream iss{nums};
    int num;
    container arr = {0};
    while (iss >> num) {
        arr[num]++;
    }

    return arr;
}

auto ifs = au::get_ifstream("inputs/06.txt");
const auto initial_counts = make_initial_counts(ifs);
auto counts = Counts(initial_counts);

void solve_a() {
    counts.simulate_until(80);
    cout << counts.count() << '\n';
}

void solve_b() {
    counts.simulate_until(256);
    cout << counts.count() << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
