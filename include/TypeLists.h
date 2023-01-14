/**
 * @file TypeLists.h
 *
 * @brief Contains declaration of TypeList as well as helper functions
 * that implements different metaprogramming features that manipulates
 * or accesses the TypeList
 */

#ifndef TYPELIST_H
#define TYPELIST_H

#include <cstddef>
#include <type_traits>
#include <utility>
#include <iostream>

namespace EAMFUP::TypeList {

// Declares struct TypeList and NullType

struct NullType {};

template <typename L, typename R>
struct TypeList {
    using First = L;
    using Rest = R;
};

/*
 * Check if two types are the same via partial specialisation
 * "Returns" true if same, and false if not
 */
template <typename T1, typename T2>
struct IsSame {
    static constexpr bool value = false;
};

template <typename T1>
struct IsSame<T1, T1> {
    static constexpr bool value = true;
};

/////////////////////////////////////////////////

/*
 * "Returns" type at given index N in the TypeList (TL)
 *
 */
template <typename TL, std::size_t N>
struct AtIndexImpl {
    using type = typename AtIndexImpl<typename TL::Rest, N - 1>::type;
};

template <std::size_t N>
struct AtIndexImpl<NullType, N>;

template <typename TL>
struct AtIndexImpl<TL, 0> {
    using type = typename TL::First;
};

template <typename TL, std::size_t N>
using AtIndex = typename AtIndexImpl<TL, N>::type;

/////////////////////////////////////////////////

/*
 * "Returns" length of Typelist including NullType
 *
 */
template <typename TL>
struct TypeListLengthImpl {
    static constexpr std::size_t value =
            1 + TypeListLengthImpl<typename TL::Rest>::value;
};

template <>
struct TypeListLengthImpl<NullType> {
    static constexpr std::size_t value = 0;
};

template <typename TL>
constexpr std::size_t TypeListLength = TypeListLengthImpl<TL>::value;

/////////////////////////////////////////////////

/**
 * "Returns" length of Typelist including NullType
 *
 */
template <typename First, typename... Rest>
struct CreateTypeListImpl {
    using type = TypeList<First, typename CreateTypeListImpl<Rest...>::type>;
};

template <typename First>
struct CreateTypeListImpl<First, NullType> {
    using type = TypeList<First, NullType>;
};

template <typename... Ts>
using CreateTypeList = typename CreateTypeListImpl<Ts..., NullType>::type;

/////////////////////////////////////////////////
/*
 * "Returns" true if InputType of a component is the same as OutputType of a
 * previous component
 *
 */
template <typename First, typename Rest>
struct MatchConnectionsImpl {
    static constexpr bool value = IsSame<typename First::OutputType,
                                         typename Rest::First::InputType>::value
            && MatchConnectionsImpl<typename Rest::First,
                                    typename Rest::Rest>::value;
};
template <typename First>
struct MatchConnectionsImpl<First, NullType> {
    static constexpr bool value = true;
};

template <typename TL>
constexpr bool MatchConnections =
        MatchConnectionsImpl<typename TL::First, typename TL::Rest>::value;

/////////////////////////////////////////////////

} // namespace EAMFUP

#endif /* ifndef TYPELIST_H */
