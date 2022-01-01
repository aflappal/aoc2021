#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <ranges>
#include <cctype>
#include <algorithm>
#include <utility>
#include "util.h"

using std::cout, std::cerr;

using Graph = std::map<std::string, std::vector<std::string>>;
using Cache = std::map<std::tuple<std::string, std::set<std::string>, bool>, int>;

auto parse() {
    auto ifs = au::get_ifstream("inputs/12.txt");
    Graph graph;
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
    // With the given input it's enough to check the first character
    return std::islower(node[0]);
}

int paths_to_end(const Graph& graph, const std::string& node,
                 std::set<std::string>& visited, Cache& cache,
                 bool visited_twice) {
    if (node == "end")
        return 1;

    const std::tuple cache_key = {node, visited, visited_twice};
    auto cache_it = cache.find(cache_key);
    if (cache_it != cache.end()) {
        return cache_it->second;
    }

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
    for (const auto& neigh : graph.at(node) | std::views::filter(can_visit)) {
        paths += paths_to_end(graph, neigh, visited, cache, visited_twice);
    }

    if (!visited_this_twice)
        visited.erase(node);
    cache[cache_key] = paths;
    return paths;
}

void solve_a() {
    const auto graph = input;
    auto visited = std::set<std::string>();
    Cache cache;
    // visited_twice = true from the start to visit small nodes only once
    cout << paths_to_end(graph, "start", visited, cache, true) << '\n';
}

void solve_b() {
    const auto graph = input;
    auto visited = std::set<std::string>();
    Cache cache;
    cout << paths_to_end(graph, "start", visited, cache, false) << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    au::solve_runner(solve_a, solve_b);
}
