/**
 * @file Distortion.h
 *
 * @brief Contains the class Distortion which is a basic distortion
 * EffectComponent
 */

// Inspired by: Joshua D. Reiss, Andrew P. McPherson:
//              Audio Effects - Theory, Implementation and Application v 1.7
//              CRC Press
//              2015
#ifndef DISTORTION_H
#define DISTORTION_H

#include <cmath>
#include <cstring>
#include <vector>
#include "Literals.h"
#include "ComponentTraits.h"
#include <math.h>
#include <memory_resource>

namespace {
// Function used to calculate datasize. Only here to demonstrate knowledge of
// PMR
consteval int getX(float dataSize) {
    int x = 0;
    while (dataSize >= static_cast<float>(pow(2.0f, x))) {
        ++x;
    }
    ++x;
    return x;
}
} // namespace

namespace EAMFUP::Component {

using namespace Litterals;

template <typename SampleType, Decibel input_gain_dB, Hertz fs, Minutes min>
    requires(input_gain_dB >= 0_dB)
class Distortion : public Traits::ComponentTraits<Traits::EffectComponentTag> {
public:
    using InputType = SampleType;
    using OutputType = SampleType;

    Distortion() noexcept {}

    void run(std::vector<InputType>& v) {
        std::vector<uint8_t> data; // samples sec == 4 min multiplied by 2 to
                                   // make sure it fits

        // Allocate enough for 'min' minutes else use new/delete
        static constexpr float dataSize =
                static_cast<float>(min.value) * 2.0f * 4.0f * 60.0f * fs.value;

        static constexpr int a = getX(dataSize);

        data.resize(pow(2, a));
        std::pmr::monotonic_buffer_resource pool(
                data.data(), data.size()); // Construct memory resource
        std::pmr::vector<float> new_data(
                &pool); // Link vector to use the memory resource

        for (auto const& element : v) { // Processing loop. Uses soft clipping
                                        // based on quadratic function
            auto x = element * inputgain_;

            if (x > threshold_upper_)
                x = 1.0f;
            else if (x > threshold_lower_)
                x = (3.0f - (2.0f - 3.0f * x) * (2.0f - 3.0f * x)) / 3.0f;
            else if (x < -threshold_upper_)
                x = -1.0f;
            else if (x < -threshold_lower_)
                x = -(3.0f - (2.0f + 3.0f * x) * (2.0f + 3.0f * x)) / 3.0f;
            else
                x = 2.0f * x;

            new_data.push_back(x);
        }

        std::memcpy(v.data(), new_data.data(),
                    new_data.size()
                            * sizeof(SampleType)); // Data copied to vector.
                                                   // Number of bytes depending
                                                   // on bit depth (sampletype)
    }

private:
    static constexpr float threshold_lower_ =
            1.0f / 3.0f; // Variables for distortion
    static constexpr float threshold_upper_ = 2.0f / 3.0f;
    static constexpr float inputgain_ =
            std::pow(10.0f, input_gain_dB.value / 20.0f);
};

} // namespace EAMFUP::Component

#endif /* DISTORTION_H */
