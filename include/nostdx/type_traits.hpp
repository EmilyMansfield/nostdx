#ifndef EM_NOSTDX_TYPE_TRAITS_HPP
#define EM_NOSTDX_TYPE_TRAITS_HPP

#include <type_traits>

namespace nostdx {

/// Provides a member type `type` equal to `T`.
/// Used to store a type instead of an instance of that type.
template<class T>
struct type_identity {
  using type = T;
};

/// Helper variable template for type_identity.
template<class T>
using type_identity_t = typename type_identity<T>::type;

/// Provides the member constant `value` equal to `true` if `T` is a
/// specialization of the class template `Temp`, and `false` otherwise.
template<class T, template<class...> class Temp>
struct is_specialization : std::false_type {};

template<template<class...> class Temp, class... Args>
struct is_specialization<Temp<Args...>, Temp> : std::true_type {};

template<class T, template<class...> class Temp>
static inline constexpr bool is_specialization_v =
    is_specialization<T, Temp>::value;

template<class T, template<class, std::size_t> class Temp>
struct is_arraylike_specialization : std::false_type {};

template<template<class, std::size_t> class Temp, class Arg, std::size_t N>
struct is_arraylike_specialization<Temp<Arg, N>, Temp> : std::true_type {};

template<class T, template<class, std::size_t> class Temp>
static inline constexpr bool is_arraylike_specialization_v =
    is_arraylike_specialization<T, Temp>::value;

}  // namespace nostdx

#endif
