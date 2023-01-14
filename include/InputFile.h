/**
 * @file Inputfile.h
 *
 * @brief Contains the inputFile class which is an InputComponent that swaps
 * data from file with data in input-vector.
 */

#ifndef INPUTFILE_H
#define INPUTFILE_H
#include <iostream>
#include <sndfile.h>
#include <string_view>
#include "ComponentTraits.h"
#include "SoundFile.h"
#include "StringLitteral.h"
#include <vector>

namespace EAMFUP::Component {

template <typename SampleType, Helper::StringLitteral filename>
class InputFile : public Traits::ComponentTraits<Traits::InputComponentTag> {
public:
    using OutputType = SampleType;
    using InputType = SampleType;
    InputFile() noexcept = default;

    void run(std::vector<SampleType>& v) {
        SF_INFO info;
        SoundFile s(std::string{filename.value}, SoundFile::Mode::READ, &info);
        std::vector<SampleType> data{};
        data.resize(std::size_t(info.frames * info.channels));
        s.readData(data.data(), std::size_t(info.frames * info.channels));
        v.swap(data);
    }
};

} // namespace EAMFUP::Component

#endif /* ifndef INPUTFILE_H */
