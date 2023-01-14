/**
 * @file StringLitteral.h
 *
 * @brief Contains functionality for creating compiletime strings
 */

#ifndef STRINGLITTERAL_H
#define STRINGLITTERAL_H

#include <string>

namespace EAMFUP::Component::Helper {

// Found here:
// https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
template <std::size_t N>
struct StringLitteral {
    constexpr StringLitteral(char const (&str)[N]) {
        std::copy_n(str, N, value);
    }
    char value[N];
};

} // namespace EAMFUP::Component::Helper

#endif /* ifndef STRINGLITTERAL_H */
