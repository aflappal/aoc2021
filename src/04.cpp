#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include "util.h"

using std::cout, std::cerr;

//const auto vals = au::get_input_vector_from_file<std::bitset<bitwidth>>("inputs/03.txt");

struct Square {
    Square(int v) : val{v}, marked{false} {}
    bool check(int v) {
        if (v == val) {
            marked = true;
            return true;
        }
        return false;
    }
    int val;
    bool marked;
};

struct Board {
    Board(std::vector<std::vector<Square>> squares)
        : squares_(squares),
          row_hits_{0},
          col_hits_{0}
    {}

    bool check(int val) {
        for (std::size_t i = 0; i < squares_.size(); i++) {
            auto& row = squares_[i];
            for (std::size_t j = 0; j < row.size(); j++) {
                auto& square = row[j];
                if (square.check(val)) {
                    row_hits_[i]++;
                    col_hits_[j]++;
                    if (row_hits_[i] == 5 || col_hits_[j] == 5) {
                        return true;
                    }
                    break;
                }
            }
        }

        return false;
    }

    int sum_of_unmarked() const {
        int sum = 0;
        for (const auto& row : squares_) {
            for (const auto& square : row) {
                if (!square.marked) sum += square.val;
            }
        }
        return sum;
    }

    void dump() const {
        cout << "row hits: ";
        for (auto num : row_hits_) {
            cout << num << ' ';
        }
        cout << '\n';
        cout << "col hits: ";
        for (auto num : col_hits_) {
            cout << num << ' ';
        }
        cout << '\n';
        for (const auto& row : squares_) {
            for (const auto& square : row) {
                cout << square.val << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
        for (const auto& row : squares_) {
            for (const auto& square : row) {
                cout << square.marked << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }
    std::vector<std::vector<Square>> squares_;
    std::array<int, 5> row_hits_;
    std::array<int, 5> col_hits_;
};

struct BingoInput {
    BingoInput(std::istream& is) {
        std::string line;
        std::getline(is, line);
        std::replace(line.begin(), line.end(), ',', ' ');
        std::istringstream iss(line);
        nums_ = std::vector<int>{std::istream_iterator<int>{iss}, {}};

        // line should be '\n'
        while (std::getline(is, line)) {
            // skip whitespacce
            is >> std::ws;
            std::vector<std::vector<Square>> squares;
            for (int i = 0; i < 5; i++) {
                std::getline(is, line);
                iss = std::istringstream(line);
                std::vector<Square> row;
                std::transform(std::istream_iterator<int>{iss}, {},
                               std::back_inserter(row),
                               [](int val) { return Square(val);});
                squares.push_back(row);
            }
            boards_.push_back(Board(squares));
        }
    }
    std::vector<int> nums_;
    std::vector<Board> boards_;
};

auto ifs = au::get_ifstream("inputs/04.txt");
const auto input = BingoInput(ifs);

void solve_a() {
    auto [nums, boards] = input;
    for (auto& num : nums) cout << num << ' ';
    cout << '\n';
    for (auto& num : nums) {
        for (auto& board : boards) {
            if (board.check(num)) {
                board.dump();
                auto sum = board.sum_of_unmarked();
                cout << sum << "*" << num << " = " << sum * num << '\n';
                goto a_end;
            }
        }
    }
a_end: ;
}

void solve_b() {
    auto [nums, boards] = input;
    auto end = boards.end();
    for (auto& num : nums) {
        // assuming there's only one board that wins last!
        end = std::remove_if(boards.begin(), end,
                             [=](auto& board) {return board.check(num);});
        if (boards.begin() == end) {
            // Apparently the last one remains first. Is that always the case?
            auto& loser = boards.front();
            loser.dump();
            auto sum = loser.sum_of_unmarked();
            cout << sum << "*" << num << " = " << sum * num << '\n';
            break;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
