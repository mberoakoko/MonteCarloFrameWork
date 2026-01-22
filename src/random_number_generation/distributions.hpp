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

    class PoissonDistribution final : public SampledDistribution<float, unsigned int> {
    public:
        explicit PoissonDistribution(const double lambda , const std::uint64_t& expectation_samples)
            : SampledDistribution<float, unsigned>({
                .sampler = [lam = lambda]() {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    // std::cout<< "Lambda "<< lambda << std::endl;
                    std::poisson_distribution<> distrib(lam);
                    return distrib(gen);
                },
                .expectation_samples = expectation_samples
            }), lambda_(lambda) {}
    private:
        double lambda_;
    };

    class GaussianDistribution final : public SampledDistribution<float, unsigned int> {
    public:
        struct DistParams {
            double mu{};
            double sigma{};
            std::size_t expectation_samples = 1000;
        };
        explicit GaussianDistribution(const double& mu , const double& sigma, const std::uint64_t& expectation_samples)
            : SampledDistribution<float, unsigned>({
                .sampler = [mu, sigma]() {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::normal_distribution<> distrib(mu, sigma);
                    return distrib(gen);
                },
                .expectation_samples = expectation_samples
            }), mu_(mu), sigma_(sigma){};

        explicit GaussianDistribution(const DistParams& params):GaussianDistribution(params.mu, params.sigma, params.expectation_samples){}

        auto get_mu() const ->  double { return mu_; }
        auto get_sigma() const -> double { return sigma_; }
    private:
        double mu_;
        double sigma_;
    };
    class GammaDistribution final : public SampledDistribution<float, unsigned int> {
    public:
        GammaDistribution(const double& alpha , const double& beta, const std::uint64_t& expectation_samples)
            :SampledDistribution<float, unsigned>({
                .sampler = [alpha, beta]() {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::gamma_distribution<> distrib(alpha, beta);
                    return distrib(gen);
                },
                .expectation_samples = expectation_samples
            }), alpha_(alpha), beta_(beta) {}

        auto get_alpha() const ->  double { return alpha_; }
        auto get_beta() const ->  double { return beta_; }

    private:
        double alpha_;
        double beta_;
    };

    class BetaDistribution final : public SampledDistribution<float, unsigned int> {
    public:
        explicit BetaDistribution(const double& alpha, const double& beta, const std::uint64_t& expectation_samples)
            : SampledDistribution<float, unsigned>({
                .sampler = [alpha, beta, expectation_samples]()-> double {
                    GammaDistribution x_dist(alpha, 1, expectation_samples);
                    GammaDistribution y_dist(beta, 1, expectation_samples);
                    double x = x_dist.sample();
                    double y = y_dist.sample();
                    return x / (x + y);
                },
                .expectation_samples = expectation_samples
            }), beta_(beta), alpha_(alpha) {}
    private:
        double alpha_;
        double beta_;
    };

}
#endif //DISTRIBUTIONS_HPP
