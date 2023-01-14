/**
 * @file ComponentTraits.h
 *
 * @brief Contains metaprogramming concepts that implement requirements
 * for components.
 */

#ifndef COMPONENTTRAITS_H
#define COMPONENTTRAITS_H

#include <concepts>
#include <type_traits>
#include <vector>
#include "TypeLists.h"

namespace EAMFUP::Component::Traits {

struct InputComponentTag {};
struct OutputComponentTag {};
struct EffectComponentTag {};
struct PipelineComponentTag {};

template <typename T>
struct ComponentTraits;

template <>
struct ComponentTraits<InputComponentTag> {
    static constexpr bool hasOutput = true;
};

template <>
struct ComponentTraits<OutputComponentTag> {
    static constexpr bool hasInput = true;
};

template <>
struct ComponentTraits<EffectComponentTag>
        : public ComponentTraits<InputComponentTag>,
          public ComponentTraits<OutputComponentTag> {};

template <>
struct ComponentTraits<PipelineComponentTag>
        : public ComponentTraits<EffectComponentTag> {};

template <typename Input>
concept isInputComponent = Input::hasOutput
        && std::derived_from<Input, ComponentTraits<InputComponentTag>>;

template <typename Effect>
concept isEffectComponent = Effect::hasOutput && Effect::hasInput
        && std::derived_from<Effect, ComponentTraits<EffectComponentTag>>;
        
template <typename Pipeline>
concept isPipelineComponent = Pipeline::hasOutput && Pipeline::hasInput
        && std::derived_from<Pipeline, ComponentTraits<PipelineComponentTag>>;

template <typename Output>
concept isOutputComponent = Output::hasInput
        && std::derived_from<Output, ComponentTraits<OutputComponentTag>>;

template <typename Component, typename SampleType>
concept isComponent = (isInputComponent<Component>
                       || isOutputComponent<Component>
                       || isEffectComponent<Component>)
        && requires(Component i, std::vector<SampleType>& v) {
               { i.run(v) } -> std::same_as<void>;
           };

// Recreating the run function requirement as above using more traditional
// metaprogramming techniques
template <typename... Ts>
using void_t = void;

template <typename Component, typename SampleType, typename = void>
struct HasRun : std::false_type {};

template <typename Component, typename SampleType>
struct HasRun<Component, SampleType,
              void_t<decltype(std::declval<Component>().run(
                      std::declval<std::vector<SampleType>&>()))>>
        : std::true_type {};

} // namespace EAMFUP::Component::Traits

#endif /* ifndef COMPONENTTRAITS_H */
