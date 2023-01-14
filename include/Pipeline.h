/**
 * @file Pipeline.h
 *
 * @brief Contains the Pipeline class, which is a variadic template that
 * constructs a serial audio processing pipeline from the given componenttypes
 */

#ifndef PIPELINE_H
#define PIPELINE_H
#include "TypeLists.h"
#include "ComponentTraits.h"
#include <algorithm>
#include <array>
#include <type_traits>
#include <utility>
#include <variant>

namespace EAMFUP::Component {

template <typename SampleType, typename... Components>
    requires(sizeof...(Components) >= 1)
        && (std::is_nothrow_default_constructible_v<Components> && ...)
        && (TypeList::MatchConnections<TypeList::CreateTypeList<Components...>>)
        && (Traits::isComponent<Components, typename Components::InputType>
            && ...)
        && (!Traits::isPipelineComponent<Components> && ...)
        && (Traits::HasRun<Components, typename Components::InputType>::value
            && ...)
class Pipeline : public Traits::ComponentTraits<Traits::PipelineComponentTag> {
    using TL = TypeList::CreateTypeList<Components...>;
    std::array<std::variant<Components...>, TypeList::TypeListLength<TL>> pipe_;

    /**  Default construct a component object of every type in TypeList from
     *   Index-begin to Index-end and store it in given storage container.
     *   Uses SFINAE principle (Selects overload depending on begin and end)
     *
     * @param s Container that should hold objects
     * (std::array)<std::variant<Components..>
     */
    template <std::size_t begin, std::size_t end>
    std::enable_if_t<(begin < end)> setupComponents() {
        using T = TypeList::AtIndex<TL, begin>;
        pipe_[begin] = T{};
        setupComponents<begin + 1, end>();
    }

    template <std::size_t begin, std::size_t end>
    std::enable_if_t<(begin == end)> setupComponents() {
        using T = TypeList::AtIndex<TL, begin>;
        pipe_[end] = T{};
    }

public:
    using InputType = SampleType;
    using OutputType = SampleType;
    Pipeline() noexcept {
        setupComponents<0, TypeList::TypeListLength<TL> - 1>();
    }

    void run(std::vector<InputType>& v) {
        std::for_each(pipe_.begin(), pipe_.end(), [&v](auto& component) {
            std::visit([&v](auto& c) { c.run(v); }, component);
        });
    }
};

} // namespace EAMFUP::Component

#endif /* ifndef PIPELINE_H */
