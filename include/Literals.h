/**
 * @file Literals.h
 *
 * @brief Contains implementation of user defined literals for Hz, Pct and dB
 */

// Inspired by: Joshua D. Reiss, Andrew P. McPherson:
//              Audio Effects - Theory, Implementation and Application v 1.7
//              CRC Press
//              2015
#ifndef LITERALS_H
#define LITERALS_H

#include <cstddef>
#include <cassert>
#include <compare>

namespace EAMFUP::Component::Litterals {

struct Hertz {
    constexpr explicit Hertz(
            std::size_t v) // Validation, if above 20.000Hz it's probably a
            : value((v <= 20'000 // samplingfrequency which is locked at
                                 // specific
                     || (v == 44'100 || v == 48'000
                         || v == 96'000 // values. Constexpr for compiletime
                                        // evaluation.
                         || v == 192'000))
                            ? v
                            : 0) {}

    auto operator<=>(Hertz const& other) const = default;

    std::size_t const value;
};

inline constexpr Hertz operator""_Hz(unsigned long long v) { return Hertz(v); }

struct Minutes {
    constexpr explicit Minutes(long double v) : value(v > 0 ? v : 0) {}

    auto operator<=>(Minutes const& other) const = default;

    long double const value;
};

inline constexpr Minutes operator""_Min(long double v) { return Minutes(v); }

struct Percent {
    constexpr explicit Percent(std::size_t v) : value((v <= 100) ? v : 100) {}

    auto operator<=>(Percent const& other) const = default;

    std::size_t const value;
};

inline constexpr Percent operator""_Pct(unsigned long long v) {
    return Percent(v);
}

struct Decibel {
    constexpr explicit Decibel(std::size_t v) : value((v <= 40) ? v : 10) {}

    auto operator<=>(Decibel const& other) const = default;

    std::size_t const value;
};

inline constexpr Decibel operator""_dB(unsigned long long v) {
    return Decibel(v);
}

} // namespace EAMFUP::Component::Litterals

#endif /* ifndef LITERALS_H */
