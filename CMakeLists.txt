cmake_minimum_required(VERSION 3.22.1)
project(MonteCarloFrameWork)

set(CMAKE_CXX_STANDARD 23)

add_executable(MonteCarloFrameWork main.cpp
        src/random_number_generation/random_number_generators.hpp
        src/sims/one_factor_monte_carlo.hpp
        src/random_number_generation/distributions.hpp)
