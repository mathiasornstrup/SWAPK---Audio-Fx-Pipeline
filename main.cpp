#include "ComponentTraits.h"
#include "PassThrough.h"
#include "Pipeline.h"
#include "Literals.h"
#include "TypeLists.h"
#include "LowPass.h"
#include "InputFile.h"
#include "Parallel.h"
#include "OutputFile.h"
#include "PassThrough.h"
#include "Tremolo.h"
#include "Distortion.h"
#include "DebugResource.h"
#include "SoundFile.h"

#include <iostream>

namespace EAMFUP {

using namespace Component::Litterals;

void batchProcess(std::string&& input_file_name,
                  std::string&& output_file_name) {
    std::vector<float> v;
    SF_INFO info;
    {
        try {
            Component::SoundFile in(input_file_name,
                                    Component::SoundFile::Mode::READ, &info);
            v.resize(static_cast<std::size_t>(info.channels * info.frames));
            in.readData(v.data(), v.size());
        } catch (Component::SoundFileException& e) {
            std::cout << e.what() << '\n';
            throw;
        }
    }

    Component::Pipeline<
            float,
            Component::Parallel<
                    float,
                    Component::Parallel<
                            float, Component::LowPass<float, 200_Hz, 48'000_Hz>,
                            Component::Distortion<float, 10_dB, 48'000_Hz,
                                                  2.0_Min>>,
                    Component::Parallel<
                            float, Component::PassThrough<float, std::vector>,
                            Component::Tremolo<float, 10_Hz, 100_Pct,
                                               48'000_Hz>>>>
            p;
    p.run(v);

    {
        Component::SoundFile out(output_file_name,
                                 Component::SoundFile::Mode::WRITE, &info);
        out.writeData(v.data(), v.size());
    }
}

void compileTimePipeline() {
    std::vector<float> v{};

    Component::Pipeline<
            float, Component::InputFile<float, "./Songs/white_noise.wav">,
            Component::Distortion<float, 10_dB, 48'000_Hz, 5.0_Min>,
            Component::Parallel<
                    float,
                    Component::Pipeline<
                            float,
                            Component::Tremolo<float, 5_Hz, 100_Pct, 48'000_Hz>,
                            Component::LowPass<float, 2000_Hz, 48'000_Hz>>,
                    Component::Distortion<float, 5_dB, 48'000_Hz, 5.0_Min>>,
            Component::OutputFile<float, 48'000_Hz, "./Songs/Out.wav">>
            p;
    p.run(v);
}

void help() {
    std::cout << "Audio Effect Pipeline\n"
              << "Usage:\n"
              << "\tNo arguments: Apply pipeline to Compile time specified "
                 "files\n"
              << "\t2 arguments:\n"
              << "\t\tInput Filename, Output filename\n"
              << "\t\tApply pipeline to runtime specified files\n";
}

} // namespace EAMFUP

auto main(int argc, char** argv) -> int {
    EAMFUP::Component::DebugResource dbg("New/Delete");
    std::pmr::set_default_resource(&dbg);
    if (argc == 1) {
        EAMFUP::compileTimePipeline();
    } else if (argc == 3) {
        std::string input_file{argv[1]};
        std::string output_file{argv[2]};
        EAMFUP::batchProcess(std::move(input_file), std::move(output_file));
    } else {
        EAMFUP::help();
    }
}
