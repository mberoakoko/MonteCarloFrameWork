//
// Created by mbero on 08/06/2025.
//

#ifndef DISTRIBUTIONS_HPP
#define DISTRIBUTIONS_HPP
#include <bits/stdc++.h>

#include <utility>
namespace Distributions {

    template<typename A, typename B>
    class Distribution {
    public:
        virtual ~Distribution() = default;
        virtual auto sample() -> A = 0;
        auto sample_n(const std::size_t& N) -> std::vector<A> {
            std::vector<A> generated_samples(N);
            std::ranges::generate(generated_samples, [&](){return sample();});
            return generated_samples;;
        }

        virtual auto expectation(std::function<double(A)> f) -> float = 0;
    };

    template<typename A, typename B>
    class SampledDistribution: public Distribution<A, B> {
    public:
        struct SampledDistParams {
            std::function<A()> sampler;
             std::uint64_t expectation_samples;
        };
        explicit SampledDistribution(std::function<A()> sampler,const std::uint64_t& expectation_samples)
        :   sampler_(std::move(sampler)),
            expectation_samples_(expectation_samples) {

        }
        explicit SampledDistribution(const SampledDistParams& params)
        :   SampledDistribution(params.sampler,params.expectation_samples) {}

        auto sample() -> A override {
            return sampler_();
        }

        auto expectation(std::function<double(A)> f) -> float override {
            // auto applied_func = this->sample_n(expectation_samples_)
            //                     | std::ranges::views::transform(f);
            std::vector<A> samples = this->sample_n(expectation_samples_);
            std::vector<double> applied_func;
            std::transform(std::begin(samples), std::end(samples), std::back_inserter(applied_func), f);
            auto sum = std::accumulate(applied_func.begin(), applied_func.end(), 0.0);
            return sum / expectation_samples_;
        }


    protected:
        std::function<A()> sampler_;
        std::uint64_t expectation_samples_ { 10 };
    };

    class UniformDistribution final : public SampledDistribution<float, unsigned int> {
    public:
        explicit UniformDistribution(const std::uint64_t& expectation_samples)
        : SampledDistribution<float, unsigned>({
                .sampler = []() -> double {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> distrib;
                    return distrib(gen);
                },
                .expectation_samples = expectation_samples
        }) {}
    };
}
#endif //DISTRIBUTIONS_HPP
