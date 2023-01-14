/**
 * @file Tremolo.h
 *
 * @brief Contains the class Tremolo which is a basic tremolo EffectComponent
 */

// Inspired by: Joshua D. Reiss, Andrew P. McPherson:
//              Audio Effects - Theory, Implementation and Application v 1.7
//              CRC Press
//              2015
#ifndef TREMOLO_H
#define TREMOLO_H

#include "ComponentTraits.h"
#include "Literals.h"
#include <vector>

namespace EAMFUP::Component { // Part of namespace Components

using namespace Litterals;

template <typename SampleType, Litterals::Hertz f_speed, Percent depth,
          Hertz fs>
    requires(depth >= 0_Pct) && (depth <= 100_Pct)
        && (f_speed
            >= 0_Hz) // Requirements for depth and f_speed template parameters
class Tremolo : public Traits::ComponentTraits<Traits::EffectComponentTag> {
public:
    using InputType = SampleType;  // Declarations for InputType
    using OutputType = SampleType; // and OutPUtType used by MatchConnection

    Tremolo() noexcept : phase_(0.0f) {} // Default ctor

    void run(std::vector<InputType>&
                     v) { // Mandatory run-function. LFO triangle wave sample at
        for (auto it = v.begin(); it != v.end();
             ++it) { // given phase is multiplied by inputsample.
            *it = (*it) * (1.0f - depth_ * lfo(phase_));

            phase_ += f_speed.value
                    * samplingtime_; // Update fase according to f_speed
            if (phase_ >= 1.0f) phase_ -= 1.0f; // Ensure overflow
        }
    }

private:
    float phase_;
    static constexpr float depth_ = depth.value / 100.0f;
    static constexpr float samplingtime_ = 1.0f / static_cast<float>(fs.value);

    // Private function that returns sample from a triangle wave LFO depending
    // on phase
    float lfo(float phase) {
        if (phase < 0.25f)
            return 0.5f + 2.0f * phase_;
        else if (phase < 0.75f)
            return 1.0f - 2.0f * (phase_ - 0.25f);
        else
            return 2.0f * (phase_ - 0.75f);
    }
};

} // namespace EAMFUP::Component
#endif
