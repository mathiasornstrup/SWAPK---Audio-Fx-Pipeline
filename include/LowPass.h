/**
 * @file LowPass.h
 *
 * @brief Contains the LowPass class which is an effectcomponent.
 * Implements a biquad lowpass filter, from the given cutoff-frequency
 * and samplefrequency.
 */

#ifndef LOWPASS_H
#define LOWPASS_H
#include <cmath>
#include <vector>
#include <array>
#include <iterator>
#include <iostream>
#include "Literals.h"
#include "ComponentTraits.h"

namespace EAMFUP::Component {

namespace {

using namespace Litterals;

template <Hertz fcutoff, Hertz fs>
    requires(fcutoff <= 20'000_Hz)
struct BiQuadKoeffs {
    static constexpr double Q = 0.51;

    static constexpr double omega0 =
            (2 * M_PI * static_cast<double>(fcutoff.value))
            / static_cast<double>(fs.value);
    static constexpr double cosine_omega0 = cos(omega0);
    static constexpr double sine_omega0 = sin(omega0);
    static constexpr double alpha = sin(sine_omega0) / (2.0 * Q);

    // Low pass filter:
    static constexpr double a0 = (1.0 + alpha);
    static constexpr double a1 = (-2.0 * cosine_omega0) / a0;
    static constexpr double a2 = (1.0 - alpha) / a0;

    static constexpr double b0 = ((1.0 - cosine_omega0) / 2) / a0;
    static constexpr double b1 = (1.0 - cosine_omega0) / a0;
    static constexpr double b2 = ((1.0 - cosine_omega0) / 2) / a0;
};

template <typename T>
struct DelayLine {
    std::array<T, 2> x = {0, 0};
    std::array<T, 2> y = {0, 0};
};

}; // namespace

template <typename SampleType, Hertz fcutoff, Hertz fs>
class LowPass : public Traits::ComponentTraits<Traits::EffectComponentTag> {
public:
    using InputType = SampleType;
    using OutputType = SampleType;

    LowPass() noexcept = default;
    void run(std::vector<SampleType>& v) {
        using Coeffs = BiQuadKoeffs<fcutoff, fs>;
        for (auto& x : v) {
            accum_ = Coeffs::b0 * static_cast<double>(x) + Coeffs::b1 * dl_.x[0]
                    + Coeffs::b2 * dl_.x[1] - Coeffs::a1 * dl_.y[0]
                    - Coeffs::a2 * dl_.y[1];

            dl_.y[1] = dl_.y[0];
            dl_.y[0] = accum_;
            dl_.x[1] = dl_.x[0];
            dl_.x[0] = x;

            x = float(accum_);
        }
    }

private:
    DelayLine<double> dl_{};

    double accum_{0.0};
};

}; // namespace EAMFUP::Component

#endif /* ifndef LOWPASS_H */
