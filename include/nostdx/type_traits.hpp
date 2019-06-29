#ifndef EM_NOSTDX_TYPE_TRAITS_HPP
#define EM_NOSTDX_TYPE_TRAITS_HPP

namespace nostdx {

/// Provides a member type `type` equal to `T`.
/// Used to store a type instead of an instance of that type.
template<class T>
struct type_identity {
  using type = T;
};

/// Helper variable template for type_identity.
template<class T> using type_identity_t = typename type_identity<T>::type;

} // namespace nostdx

#endif
