/**
 * @file OutputFile.h
 *
 * @brief Contains the OutputFile class which is a OutputComponent
 * that writes an audiofile from the given vector v.
 */

#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H
#include <sndfile.h>
#include <cstdio>
#include <string>
#include "ComponentTraits.h"
#include "StringLitteral.h"
#include "SoundFile.h"
#include "Literals.h"
#include <type_traits>
#include <vector>
#include <iostream>

namespace EAMFUP::Component {

template <typename SampleType, Litterals::Hertz sample_rate,
          Helper::StringLitteral string>
class OutputFile : public Traits::ComponentTraits<Traits::OutputComponentTag> {
    void updateFormat(int& format) {
        if (std::is_same_v<SampleType, float>) {
            format |= SF_FORMAT_FLOAT;
        } else if (std::is_same_v<SampleType, int16_t>) {
            format |= SF_FORMAT_PCM_16;
        }
    }

public:
    using InputType = SampleType;
    using OutputType = SampleType;
    constexpr OutputFile() noexcept = default;

    void run(std::vector<InputType>& v) {
        SF_INFO info;
        info.samplerate = sample_rate.value;
        info.channels = 1;
        info.format = SF_FORMAT_WAV;
        updateFormat(info.format);
        SoundFile s(std::string{string.value}, SoundFile::Mode::WRITE, &info);
        s.writeData(v.data(), v.size());
    }
};

} // namespace EAMFUP::Component

#endif /* ifndef OUTPUTFILE_H */
