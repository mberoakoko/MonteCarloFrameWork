//
// Created by mbero on 03/06/2025.
//

#ifndef ONE_FACTOR_MONTE_CARLO_HPP
#define ONE_FACTOR_MONTE_CARLO_HPP
#include <bits/stdc++.h>
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
}
#endif //ONE_FACTOR_MONTE_CARLO_HPP
