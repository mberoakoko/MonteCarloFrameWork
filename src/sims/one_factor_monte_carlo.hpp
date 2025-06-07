//
// Created by mbero on 03/06/2025.
//

#ifndef ONE_FACTOR_MONTE_CARLO_HPP
#define ONE_FACTOR_MONTE_CARLO_HPP
#include <bits/stdc++.h>
#include <pstl/execution_impl.h>

namespace OneFactorMonteCarlo {
    template<typename T>
    struct  Range {
        T min;
        T max;
    };

    class OneFactorSDEVisitor;

    class OneFactorSDE {
    public:
        struct Params {
            double initial_condition;
            Range<double> range;
        };
        explicit OneFactorSDE(const Params& params): ic_(params.initial_condition), range_(params.range){}
        explicit OneFactorSDE(const double& initial_condition, const Range<double>& range): ic_(initial_condition), range_(range){}

        virtual ~OneFactorSDE() = default;;
        auto get_initial_conditions() const -> double{return ic_;}
        auto get_range() const -> Range<double>{return range_;}
        virtual auto accept(const OneFactorSDEVisitor& visitor) -> void = 0;

    private:
        double ic_;
        Range<double> range_;
    };

    class SDETypeD final: public OneFactorSDE {
    public:
        using drift = std::function<double(double, double)>;
        using diffusion = std::function<double(double, double)>;
        struct SDETypeDParams {
            drift drift_func;
            diffusion diffusion_func;
        };

        ~SDETypeD() override {};

        explicit SDETypeD(const Params& params, const SDETypeDParams& sde_type_d_params)
        : OneFactorSDE(params), drift_(sde_type_d_params.drift_func), diffusion_(sde_type_d_params.diffusion_func){}
        explicit SDETypeD(const Params& params, const drift& drift_function, const diffusion& diffusion_function)
        : OneFactorSDE(params), drift_(drift_function), diffusion_(diffusion_function){}



        auto accept(const OneFactorSDEVisitor &visitor) -> void override {

        };

        auto calculate_drift(const double& t,const  double& X) const -> double {
            return drift_(t, X);
        };
        auto calculate_diffusion(const double& t,const double& X) const -> double {
            return diffusion_(t, X);
        }

    private:
        drift drift_;
        diffusion diffusion_;
    };

    template<typename T, typename R>
    using Vector = std::vector<T>;;

    class OneFactorSDEVisitor {
    public:
        explicit OneFactorSDEVisitor() = default;
        explicit OneFactorSDEVisitor(const OneFactorSDEVisitor& visitor) = default;
        virtual ~OneFactorSDEVisitor() = default;

        virtual auto visit(const SDETypeD& sde_type_d) -> void  = 0;
        // TODO: other SDE types must be implemented

        auto operator()(const OneFactorSDEVisitor& source) -> OneFactorSDEVisitor& {
            return *this;
        };
    };

    class FiniteDifferenceMethodsVisitor : public OneFactorSDEVisitor {
    public:
        explicit FiniteDifferenceMethodsVisitor() = default;
        explicit FiniteDifferenceMethodsVisitor(const std::size_t& N , const double dt,
            const double initial_value )
        :   initial_condition(initial_value), dt_(dt), num_steps(N){}

        auto set_random_vector(const std::vector<double>& random_vector) -> void {
            std::ranges::copy(random_vector, std::back_inserter(d_W));
        }

        auto get_path() -> std::vector<double> {
            return result;
        }

    protected:
        double initial_condition, v_previous {0};
        double v_current {0};
        double dt_;
        std::size_t num_steps;
        // TODO: Datastrucures to store result, path and state matrices
        std::vector<double> result;
        std::vector<double> d_W;
    };

    /*class ExplicitEuler final: FiniteDifferenceMethodsVisitor {
    public:
        explicit ExplicitEuler() = default;
        explicit ExplicitEuler(const std::size_t& N , const double dt,const double initial_value )
        : FiniteDifferenceMethodsVisitor(N,dt,initial_value){};

        auto visit(const SDETypeD &sde_type_d) -> void override {
            v_previous = initial_condition;
            result.push_back(initial_condition);
            auto euler_step = [&](std::size_t index) -> double {
                v_current = v_previous * sde_type_d.calculate_drift()
                v_previous = v_current;
            }(1);
        };

    };*/

}
#endif //ONE_FACTOR_MONTE_CARLO_HPP
