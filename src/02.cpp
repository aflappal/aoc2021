#include <iostream>
#include "util.h"

using std::cout, std::cerr;

struct CommandPair {
    std::string dir;
    int magnitude;
};

std::istream& operator>>(std::istream& stream, CommandPair& in) {
    return stream >> in.dir >> in.magnitude;
}

const auto commands = au::get_input_vector_from_file<CommandPair>("inputs/02.txt");

void solve_a() {
    auto x = 0, z = 0;
    for (auto command : commands) {
        if (command.dir == "forward") {
            x += command.magnitude;
        } else if (command.dir == "down") {
            z += command.magnitude;
        } else if (command.dir == "up") {
            z -= command.magnitude;
        } else {
            cerr << "got weird command " << command.dir << '\n';
            std::exit(2);
        }
    }

    cout << "x = " << x << ", z = " << z << ", x * z = " << x * z << '\n';
}

void solve_b() {
    auto x = 0, z = 0, aim = 0;
    for (auto command : commands) {
        if (command.dir == "forward") {
            x += command.magnitude;
            z += aim * command.magnitude;
        } else if (command.dir == "down") {
            aim += command.magnitude;
        } else if (command.dir == "up") {
            aim -= command.magnitude;
        } else {
            cerr << "got weird command " << command.dir << '\n';
            std::exit(2);
        }
    }

    cout << "x = " << x << ", z = " << z << ", x * z = " << x * z << '\n';
}

int main() {
    au::solve_runner(solve_a, solve_b);
}
