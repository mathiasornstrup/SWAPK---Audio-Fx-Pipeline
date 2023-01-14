/**
 * @file Parallel.h
 *
 * @brief Contains the Parallel class, which is a template class that
 * constructs a parallel audio processing pipeline from the given 2 components.
 */

#ifndef PARALLEL_H
#define PARALLEL_H

#include "TypeLists.h"
#include "ComponentTraits.h"
#include <thread>
#include <algorithm>
#include <iterator>
#include <future>
#include <array>
#include <type_traits>
#include <utility>
#include <variant>
#include <memory_resource>
#include <vector>

namespace EAMFUP::Component {

template <typename SampleType, typename Component1, typename Component2>
    requires(std::is_nothrow_default_constructible_v<Component1>
             && std::is_nothrow_default_constructible_v<Component2>)
        && (TypeList::MatchConnections<
                TypeList::CreateTypeList<Component1, Component2>>)
        && (Traits::isComponent<Component1, typename Component1::InputType>
            && Traits::isComponent<Component2, typename Component2::InputType>)

class Parallel : public Traits::ComponentTraits<Traits::EffectComponentTag> {
    Component1 component1;
    Component2 component2;

    std::function<void(std::vector<float>&)> path1;
    std::function<void(std::vector<float>&)> path2;
    void path2(std::vector)

public:
    using InputType = SampleType;
    using OutputType = SampleType;

    Parallel() noexcept
            : component1(),
              component2(),
              path1(std::bind(&Component1::run, component1,
                              std::placeholders::_1)),
              path2(std::bind(&Component2::run, component2,
                              std::placeholders::_1)) {}

    void run(std::vector<InputType>& v) {
        // Holds copy for one path
        std::vector<InputType> datacopy1;
        std::vector<InputType> datacopy2;

        // Try-catch block strong guarantee
        try {
            datacopy1.reserve(v.size());
            datacopy2.reserve(v.size());

            // Copy input vector
            std::copy(v.begin(), v.end(), back_inserter(datacopy1));
            std::copy(v.begin(), v.end(), back_inserter(datacopy2));

            // Run threads
            {
                std::jthread jt1(path1, std::ref(datacopy1));
                std::jthread jt2(path2, std::ref(datacopy2));
            }

            /*// Alternate version with lambdas insted of bind and function:
            {
                std::jthread jt1{[&datacopy1, this]() {
                    this->component1.run(datacopy1);
                }};

                std::jthread jt2{[&datacopy2, this]() {
                    this->component2.run(datacopy2);
                }};
            }
            */

            // mix the result
            mixdown(datacopy1, datacopy2);
        } catch (const std::length_error& le) {
            std::cerr << "File too long for parallel processing: " << le.what()
                      << '\n';
            throw;
        } catch (const std::bad_alloc& ba) {
            std::cerr << "Allocation for parallel processing failed: "
                      << ba.what() << '\n';
            throw;
        }
        std::swap(v, datacopy1);
    }

    // Simple version
    void mixdown(std::vector<float>& d1, std::vector<float>& d2) {
        for (size_t i = 0; i < d1.size(); ++i) {
            d1[i] = (d1[i] + d2[i]) / 2;
        }
    }
};

} // namespace EAMFUP::Component

#endif /* ifndef PARALLEL_H */
