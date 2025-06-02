#include <iostream>
#include <coroutine>
#include "src/random_number_generation/random_number_generators.hpp"


int main() {
    auto uniform_generator = std::make_unique<RandomNumber::UniformGenerator>();
    // RandomNumber::BoxMullerGenerator generator(uniform_generator);
    return 0;
}