//
// Created by mbero on 02/06/2025.
//

#ifndef RANDOM_NUMBER_GENERATORS_HPP
#define RANDOM_NUMBER_GENERATORS_HPP
#include <bits/stdc++.h>
namespace RandomNumber {
    class IUniformGenerator {
    public:
        virtual ~IUniformGenerator() = default;

        virtual auto init(std::uint64_t seed) -> void = 0;
        virtual auto get_uniform_value() const -> double = 0;
        [[nodiscard]]
        auto get_uniform_vector(std::size_t N) const -> std::vector<double> {
            std::vector<double> result;
            result.reserve(N);
            std::fill(result.begin(), result.end(), [this]() {
                return get_uniform_value();
            });
            return result;
        }
    };

    class INormalGenerator {
    public:
        virtual ~INormalGenerator() = default;
        virtual auto get_normal_value() const -> double = 0;
        [[nodiscard]]
        auto get_normal_vector(std::size_t N) const -> std::vector<double> {
            std::vector<double> result;
            result.reserve(N);
            std::fill(result.begin(), result.end(), [this] {
                return get_normal_value();
            });
            return result;
        }

        std::unique_ptr<IUniformGenerator> generator_ {};
    };

    class UniformGenerator final: public IUniformGenerator {
    public:
        explicit UniformGenerator() = default;
        explicit UniformGenerator(const double factor): factor_{factor}{};
        auto get_uniform_value() const -> double override {
            return std::rand();
        }

        auto init(std::uint64_t seed) -> void override {
            std::srand(seed);
        }

    private:
        double factor_{ 1 };

    };

    class BoxMullerGenerator final : public INormalGenerator {
    public:
        explicit BoxMullerGenerator(const std::unique_ptr<IUniformGenerator>& normal_generator)
        : INormalGenerator() {
            generator_.reset();
            // generator_ = normal_generator;
        };

        ~BoxMullerGenerator() override;

        auto get_normal_value() -> double  {
            u_1 = generator_->get_uniform_value();
            u_2 = generator_->get_uniform_value();
            double w = ::sqrt(- 2 * ::log(u_1));
            n_1 = w * ::cos(2 * ::std::numbers::pi * u_2);
            n_2 = w * ::sin(2 * ::std::numbers::pi * u_2);
            return n_1;
        };

    private:
        double n_1 {}, n_2{};
        double u_1 {}, u_2 {};
    };
}

#endif //RANDOM_NUMBER_GENERATORS_HPP
