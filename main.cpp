#include <iostream>
#include <coroutine>
#include "src/random_number_generation/random_number_generators.hpp"


int main() {
    auto uniform_generator = std::make_unique<RandomNumber::UniformGenerator>();
    auto result = uniform_generator->get_uniform_vector(10000);
    // std::ranges::copy(result, std::ostream_iterator<double>(std::cout, "\n"));
    RandomNumber::BoxMullerGenerator generator(std::make_unique<RandomNumber::UniformGenerator>());
    RandomNumber::BoxMullerGenerator generator_2(std::move(uniform_generator));
    auto result_2 = generator_2.get_normal_vector(1000);

    return 0;
}