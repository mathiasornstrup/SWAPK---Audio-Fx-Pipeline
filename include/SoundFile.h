/**
 * @file StringLitteral.h
 *
 * @brief Contains SoundFile which is a wrapper for libsndfile, that enables
 * reading and writing audiofile.
 */

#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <sndfile.h>
#include <string>

namespace EAMFUP::Component {

struct SoundFileException {
    SoundFileException(std::string const& message) : message_(message){};
    std::string what() { return message_; };

private:
    std::string message_;
};

struct SoundFile {
    enum class Mode { READ, WRITE };

    SoundFile(std::string const& file_name, Mode mode, SF_INFO* info) {
        file = sf_open(file_name.c_str(), modeToMacro(mode), info);
        if (file == NULL) {
            printf("Error: %s\n", sf_strerror(NULL));
            throw SoundFileException{"sf_open failed!"};
        }
        if (info->channels != 1) {
            printf("Only MONO supported!\n");
            exit(1);
        }
        if (!(info->format & SF_FORMAT_WAV)) {
            printf("Only Wav is supported!\n");
            exit(1);
        }
        if (!(info->format & SF_FORMAT_PCM_16)
            || !(info->format & SF_FORMAT_FLOAT)) {
            printf("Only int16_t and float32 are supported!\n");
            exit(1);
        }
    }

    ~SoundFile() { sf_close(file); }

    template <typename T>
    void writeData(T const* data, std::size_t size) {
        if constexpr (std::is_same_v<T, float>) {
            auto const written = sf_writef_float(file, data, sf_count_t(size));
            if (std::size_t(written) != size) {
                printf("Written different from size: written: %ld, size: %ld\n",
                       written, size);
            }
        } else if constexpr (std::is_same_v<T, int16_t>) {
            auto const written = sf_writef_short(file, data, sf_count_t(size));
            if (std::size_t(written) != size) {
                printf("Written different from size: written: %ld, size: %ld\n",
                       written, size);
            }
        }
    }

    template <typename T>
    void readData(T* data, std::size_t size) {
        if constexpr (std::is_same_v<T, float>) {
            auto const read = sf_read_float(file, data, sf_count_t(size));
            if (std::size_t(read) != size) {
                printf("Written different from size: written: %ld, size: %ld\n",
                       read, size);
            }
        } else if constexpr (std::is_same_v<T, int16_t>) {
            auto const read = sf_read_short(file, data, sf_count_t(size));
            if (std::size_t(read) != size) {
                printf("Written different from size: written: %ld, size: %ld\n",
                       read, size);
            }
        }
    }

private:
    int modeToMacro(Mode m) {
        if (m == Mode::READ) {
            return SFM_READ;
        }
        return SFM_WRITE;
    }

    SNDFILE* file;
};

} // namespace EAMFUP::Component

#endif /* ifndef SOUNDFILE_H */
