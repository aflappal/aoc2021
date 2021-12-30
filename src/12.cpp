#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <ranges>
#include <cctype>
#include <algorithm>
#include "util.h"

using std::cout, std::cerr;

using graph_type = std::map<std::string, std::vector<std::string>>;

auto parse() {
    auto ifs = au::get_ifstream("inputs/12.txt");
    graph_type graph;
    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream iss{line};
        std::string node1, node2;
        std::getline(iss, node1, '-');
        std::getline(iss, node2);
        graph[node1].push_back(node2);
        graph[node2].push_back(node1);
    }
    return graph;
}

const auto input = parse();

auto is_small(const std::string& node) {
    const auto is_lower = [](unsigned char c) { return std::islower(c); };
    return std::ranges::all_of(node, is_lower);
}

// XXX graph can't be const for some reason
int paths_to_end(graph_type& graph, const std::string& node,
                 std::set<std::string>& visited, bool visited_twice) {
    if (node == "end")
        return 1;

    bool visited_this_twice = false;
    if (is_small(node)) {
        if (visited.contains(node)) {
            visited_twice = visited_this_twice = true;
        } else {
            visited.insert(node);
        }
    }

    auto paths = 0;
    const auto can_visit = [&](const auto& n) {
        return n != "start" && (!visited_twice || !visited.contains(n));
    };
    for (const auto& neigh : graph[node] | std::views::filter(can_visit)) {
        paths += paths_to_end(graph, neigh, visited, visited_twice);
    }
    if (!visited_this_twice)
        visited.erase(node);
    return paths;
}

void solve_a() {
    auto graph = input;
    auto visited = std::set<std::string>();
    // visited_twice = true from the start to visit small nodes only once
    cout << paths_to_end(graph, "start", visited, true) << '\n';
}

void solve_b() {
    auto graph = input;
    auto visited = std::set<std::string>();
    cout << paths_to_end(graph, "start", visited, false) << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
