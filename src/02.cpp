#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

struct CommandPair {
    std::string dir;
    int magnitude;
};

std::istream& operator>>(std::istream& stream, CommandPair& in) {
    return stream >> in.dir >> in.magnitude;
}

int main() {
    std::ifstream ifs("inputs/02.txt");
    if (!ifs) {
        std::cerr << "file not found" << std::endl;
        return 1;
    }

    std::vector<CommandPair> commands(std::istream_iterator<CommandPair>{ifs},
                                      std::istream_iterator<CommandPair>{});

    auto x = 0, z = 0;
    for (auto command : commands) {
        if (command.dir == "forward") {
            x += command.magnitude;
        } else if (command.dir == "down") {
            z += command.magnitude;
        } else if (command.dir == "up") {
            z -= command.magnitude;
        } else {
            std::cerr << "got weird command " << command.dir << std::endl;
            return 2;
        }
    }

    std::cout << "x = " << x << ", z = " << z << ", x * z = " << x * z
              << std::endl;

    return 0;
}
