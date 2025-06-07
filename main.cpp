#include <iostream>
#include <coroutine>
#include "src/random_number_generation/random_number_generators.hpp"
#include "src/sims/one_factor_monte_carlo.hpp"
#include "src/random_number_generation/distributions.hpp"

namespace ExactSDE {
    // Known solution accordin to ito's lemma
    // dS = aSdt + bSdW
    // a |-> drift coeffient
    // b |-> diffusion coeffient
    double a,  b;
    static OneFactorMonteCarlo::SDETypeD::SDETypeDParams params{
        .drift_func = [](double t, const double x) -> double      {return a * x;},
        .diffusion_func = [](double t, const double x) -> double  {return b * x;}
    };
}

int main() {
    auto uniform_generator = std::make_unique<RandomNumber::UniformGenerator>();
    auto result = uniform_generator->get_uniform_vector(10000);
    // std::ranges::copy(result, std::ostream_iterator<double>(std::cout, "\n"));
    RandomNumber::BoxMullerGenerator generator(std::make_unique<RandomNumber::UniformGenerator>());
    RandomNumber::BoxMullerGenerator generator_2(std::move(uniform_generator));
    auto result_2 = generator_2.get_normal_vector(1000);

    ExactSDE::a = 1;
    ExactSDE::b = 2;

    OneFactorMonteCarlo::OneFactorSDE::Params params{.initial_condition = 0, .range = {.min = 10, .max = 100}};
    OneFactorMonteCarlo::SDETypeD::SDETypeDParams sde_type_d_params{
        .drift_func = ExactSDE::params.drift_func,
        .diffusion_func = ExactSDE::params.drift_func,
    };
    OneFactorMonteCarlo::SDETypeD sde(params, sde_type_d_params);
    std::cout<< sde.calculate_diffusion(1, 100) << std::endl;
    std::cout<< sde.calculate_drift(1, 100) << std::endl;
    Distributions::UniformDistribution uniform_distribution(100);
    std::cout<< uniform_distribution.sample() << std::endl;
    uniform_distribution.sample_n(100);
    std::cout << "Calculated Expectation Value "
        << uniform_distribution.expectation([](float input_number) -> double { return input_number; })
        << std::endl;

    return 0;
}