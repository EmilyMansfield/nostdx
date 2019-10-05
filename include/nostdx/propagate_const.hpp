#ifndef EM_NOSTDX_PROPAGATE_CONST_HPP
#define EM_NOSTDX_PROPAGATE_CONST_HPP

#include "nostdx/type_traits.hpp"
#include <utility>

namespace nostdx {

template<class T>
class propagate_const;

namespace internal {

template<class T> static inline constexpr bool
    is_propagate_const_specialization_v =
    is_specialization_v<T, propagate_const>;

} // namespace internal

/// Implementation of std::experimental::propagate_const defined in the
/// Library Fundamentals TS v2.
template<class T>
class propagate_const {
 private:
  T mT{};

 public:
  using element_type = std::remove_reference_t<decltype(*std::declval<T &>())>;

  constexpr propagate_const() = default;

  constexpr propagate_const(propagate_const &&p)
  noexcept(std::is_nothrow_move_constructible_v<T>) = default;

  template<class U, std::enable_if_t<
      std::is_constructible_v<T, U &&> && std::is_convertible_v<U &&, T>,
      bool> = true>
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  /*explicit(false)*/ constexpr propagate_const(propagate_const<U> &&pu)
  noexcept(std::is_nothrow_constructible_v<T, U &&>) : mT{std::move(pu.mT)} {}

  template<class U, std::enable_if_t<
      std::is_constructible_v<T, U &&> && !std::is_convertible_v<U &&, T>,
      bool> = true>
  constexpr explicit propagate_const(propagate_const<U> &&pu)
  noexcept(std::is_nothrow_constructible_v<T, U &&>) : mT{std::move(pu.mT)} {}

  template<class U, std::enable_if_t<
      std::is_constructible_v<T, U &&> &&
          !internal::is_propagate_const_specialization_v<std::decay_t<U>> &&
          std::is_convertible_v<U &&, T>,
      bool> = true>
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions, bugprone-forwarding-reference-overload)
  /*explicit(false)*/ constexpr propagate_const(U &&u)
  noexcept(std::is_nothrow_constructible_v<T, U &&>) : mT{std::forward<U>(u)} {}

  template<class U, std::enable_if_t<
      std::is_constructible_v<T, U &&> &&
          !internal::is_propagate_const_specialization_v<std::decay_t<U>> &&
          !std::is_convertible_v<U &&, T>,
      bool> = true>
  // NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
  constexpr explicit propagate_const(U &&u)
  noexcept(std::is_nothrow_constructible_v<T, U &&>) : mT{std::forward<U>(u)} {}

  propagate_const(const propagate_const &) = delete;

  constexpr propagate_const &operator=(propagate_const &&)
  noexcept(std::is_nothrow_move_assignable_v<T>) = default;

  template<class U, std::enable_if_t<
      std::is_constructible_v<T, U &&> && std::is_convertible_v<U &&, T>,
      bool> = true>
  constexpr propagate_const &operator=(propagate_const<U> &&pu)
  noexcept(std::is_nothrow_assignable_v<T, U &&>) {
    mT = std::move(pu.mT);
    return *this;
  }

  template<class U, std::enable_if_t<
      std::is_constructible_v<T, U &&> &&
          !internal::is_propagate_const_specialization_v<std::decay_t<U>> &&
          std::is_convertible_v<U &&, T>,
      bool> = true>
  constexpr propagate_const &operator=(U &&u)
  noexcept(std::is_nothrow_assignable_v<T, U &&>) {
    mT = std::forward<U>(u);
    return *this;
  }

  propagate_const &operator=(const propagate_const &) = delete;

  ~propagate_const() = default;

  constexpr void
  swap(propagate_const &p) noexcept(std::is_nothrow_swappable_v<T>) {
    using std::swap;
    swap(mT, p.mT);
  }

  constexpr element_type *get() {
    if constexpr (std::is_pointer_v<T>)
      return mT;
    else
      return mT.get();
  }

  constexpr element_type *get() const {
    if constexpr (std::is_pointer_v<T>)
      return mT;
    else
      return mT.get();
  }

  constexpr explicit operator bool() const { return (bool) mT; }

  constexpr element_type &operator*() { return *get(); }
  constexpr const element_type &operator*() const { return *get(); }

  constexpr element_type *operator->() { return get(); }
  constexpr const element_type *operator->() const { return get(); }

  template<class T_ = T,
      class = std::enable_if_t<std::is_convertible_v<T_, element_type *>>>
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  /*explicit(false)*/ constexpr operator element_type *() { return get(); }

  template<class T_ = T,
      class = std::enable_if_t<std::is_convertible_v<T_, const element_type *>>>
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  /*explicit(false)*/ constexpr operator const element_type *() const {
    return get();
  }
};

template<class T>
constexpr bool operator==(const propagate_const<T> &p, std::nullptr_t) {
  return p.mT == nullptr;
}

template<class T>
constexpr bool operator==(std::nullptr_t, const propagate_const<T> &p) {
  return nullptr == p.mT;
}

template<class T>
constexpr bool operator!=(const propagate_const<T> &p, std::nullptr_t) {
  return p.mT != nullptr;
}

template<class T>
constexpr bool operator!=(std::nullptr_t, const propagate_const<T> &p) {
  return nullptr != p.mT;
}

template<class T, class U>
constexpr bool
operator==(const propagate_const<T> &pt, const propagate_const<U> &pu) {
  return pt.mT == pu.mT;
}

template<class T, class U>
constexpr bool
operator!=(const propagate_const<T> &pt, const propagate_const<U> &pu) {
  return pt.mT != pu.mT;
}

template<class T, class U>
constexpr bool
operator<(const propagate_const<T> &pt, const propagate_const<U> &pu) {
  return pt.mT < pu.mT;
}

template<class T, class U>
constexpr bool
operator<=(const propagate_const<T> &pt, const propagate_const<U> &pu) {
  return pt.mT <= pu.mT;
}

template<class T, class U>
constexpr bool
operator>(const propagate_const<T> &pt, const propagate_const<U> &pu) {
  return pt.mT > pu.mT;
}

template<class T, class U>
constexpr bool
operator>=(const propagate_const<T> &pt, const propagate_const<U> &pu) {
  return pt.mT >= pu.mT;
}

template<class T, class U>
constexpr bool operator==(const propagate_const<T> &pt, const U &u) {
  return pt.mT == u;
}

template<class T, class U>
constexpr bool operator!=(const propagate_const<T> &pt, const U &u) {
  return pt.mT != u;
}

template<class T, class U>
constexpr bool operator==(const T &t, const propagate_const<U> &pu) {
  return t == pu.mT;
}

template<class T, class U>
constexpr bool operator!=(const T &t, const propagate_const<U> &pu) {
  return t != pu.mT;
}

template<class T, class U>
constexpr bool operator<(const propagate_const<T> &pt, const U &u) {
  return pt.mT < u;
}

template<class T, class U>
constexpr bool operator<=(const propagate_const<T> &pt, const U &u) {
  return pt.mT <= u;
}

template<class T, class U>
constexpr bool operator>(const propagate_const<T> &pt, const U &u) {
  return pt.mT > u;
}

template<class T, class U>
constexpr bool operator>=(const propagate_const<T> &pt, const U &u) {
  return pt.mT >= u;
}

template<class T, class U>
constexpr bool operator<(const T &t, const propagate_const<U> &pu) {
  return t < pu.mT;
}

template<class T, class U>
constexpr bool operator<=(const T &t, const propagate_const<U> &pu) {
  return t <= pu.mT;
}

template<class T, class U>
constexpr bool operator>(const T &t, const propagate_const<U> &pu) {
  return t > pu.mT;
}

template<class T, class U>
constexpr bool operator>=(const T &t, const propagate_const<U> &pu) {
  return t >= pu.mT;
}

template<class T>
constexpr void swap(propagate_const<T> &lhs, propagate_const<T> &rhs)
noexcept(std::is_nothrow_swappable_v<T>) {
  return lhs.swap(rhs);
}

template<class T>
constexpr T &get_underlying(propagate_const<T> &pt) noexcept {
  return pt.mT;
}

template<class T>
constexpr const T &get_underlying(const propagate_const<T> &pt) noexcept {
  return pt.mT;
}

} // namespace nostdx

#endif
