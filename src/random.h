#pragma once

#include <random>

namespace au::random {

template <typename IntType = int>
class UniformRand {
public:
    UniformRand(IntType a, IntType b) : rd{}, re{rd()}, dist{a, b} {}
    // Return random num in range [a, b]
    IntType rand() {
        return dist(re);
    }
private:
    std::random_device rd;
    std::default_random_engine re;
    std::uniform_int_distribution<> dist;
};

template <class IntType = int>
IntType rand(IntType a, IntType b) {
    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_int_distribution<IntType> dist(a, b);
    return dist(re);
}

} // namespace au::random
