#ifndef EM_NOSTDX_SPAN_HPP
#define EM_NOSTDX_SPAN_HPP

#include "nostdx/type_traits.hpp"
#include <array>
#include <iterator>
#include <limits>
#include <tuple>

namespace nostdx {

template<class T, std::size_t Extent>
class span;

namespace internal {

template<class T>
static inline constexpr bool is_array_specialization_v =
    is_arraylike_specialization_v<T, std::array>;

template<class T>
static inline constexpr bool is_span_specialization_v =
    is_arraylike_specialization_v<T, nostdx::span>;

}  // namespace internal

inline constexpr std::size_t dynamic_extent =
    std::numeric_limits<std::size_t>::max();

template<class T, std::size_t Extent = dynamic_extent>
class span {
 public:
  using element_type = T;
  using value_type = std::remove_cv_t<T>;
  using index_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const iterator>;

  static constexpr std::size_t extent = Extent;

 private:
  pointer mData;
  index_type mSize;

 public:
  template<
      std::size_t Extent_ = Extent,
      std::enable_if_t<Extent_ == 0 || Extent_ == dynamic_extent, bool> = true>
  constexpr span() noexcept : mData{nullptr}, mSize{} {}

  constexpr span(pointer ptr, index_type count) : mData{ptr}, mSize{count} {
    static_assert(extent == dynamic_extent || extent == count);
  }

  constexpr span(pointer first, pointer last)
      : mData{first}, mSize{last - first} {}

  template<
      std::size_t N,
      std::enable_if_t<(extent == dynamic_extent || extent == N), bool> = true>
  constexpr span(element_type (&arr)[N]) noexcept
      : mData{std::data(arr)}, mSize{N} {}

  template<
      std::size_t N,
      std::enable_if_t<(extent == dynamic_extent || extent == N), bool> = true>
  constexpr span(std::array<value_type, N> &arr) noexcept
      : mData{std::data(arr)}, mSize{N} {}

  template<
      std::size_t N,
      std::enable_if_t<(extent == dynamic_extent || extent == N), bool> = true>
  constexpr span(const std::array<value_type, N> &arr) noexcept
      : mData{std::data(arr)}, mSize{N} {}

  template<
      class Container,
      std::enable_if_t<
          !internal::is_array_specialization_v<Container> &&
              !internal::is_span_specialization_v<Container> &&
              !std::is_array_v<Container> &&
              std::is_convertible_v<std::remove_pointer_t<decltype(std::data(
                                        std::declval<Container &>()))> (*)[],
                                    element_type (*)[]>,
          bool> = true,
      class = std::void_t<decltype(std::size(std::declval<Container &>()))>>
  constexpr span(Container &container)
      : mData{std::data(container)}, mSize{std::size(container)} {}

  template<
      class Container,
      std::enable_if_t<!internal::is_array_specialization_v<Container> &&
                           !internal::is_span_specialization_v<Container> &&
                           !std::is_array_v<Container> &&
                           std::is_convertible_v<
                               std::remove_pointer_t<decltype(std::data(
                                   std::declval<const Container &>()))> (*)[],
                               element_type (*)[]>,
                       bool> = true,
      class =
          std::void_t<decltype(std::size(std::declval<const Container &>()))>>
  constexpr span(const Container &container)
      : mData{std::data(container)}, mSize{std::size(container)} {}

  template<
      class U, std::size_t N,
      std::enable_if_t<(extent == dynamic_extent || extent == N) &&
                           std::is_convertible_v<U (*)[], element_type (*)[]>,
                       bool> = true>
  constexpr span(const span<U, N> &s) noexcept
      : mData{s.mData}, mSize{s.mSize} {}

  constexpr span(const span &s) noexcept = default;
  constexpr span &operator=(const span &s) noexcept = default;

  constexpr iterator begin() const noexcept { return mData; }
  constexpr const_iterator cbegin() const noexcept { return mData; }

  constexpr iterator end() const noexcept { return mData + mSize; }
  constexpr const_iterator cend() const noexcept { return mData + mSize; }

  constexpr reverse_iterator rbegin() const noexcept {
    return reverse_iterator(end());
  }
  constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }

  constexpr reverse_iterator rend() const noexcept {
    return reverse_iterator(begin());
  }

  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  friend constexpr iterator begin(span s) noexcept { return s.begin(); }
  friend constexpr iterator end(span s) noexcept { return s.end(); }

  constexpr reference front() const { return *begin(); }
  constexpr reference back() const { return *(end() - 1); }
  constexpr reference operator[](index_type idx) const { return mData[idx]; }
  constexpr pointer data() const noexcept { return mData; }
  constexpr index_type size() const noexcept { return mSize; }
  constexpr index_type size_bytes() const noexcept { return mSize * sizeof(T); }
  [[nodiscard]] constexpr bool empty() const noexcept { return mSize == 0; }

  template<std::size_t N>
  constexpr span<element_type, N> first() const {
    return {mData, N};
  }

  constexpr span<element_type, dynamic_extent> first(index_type count) const {
    return {mData, count};
  }

  template<std::size_t N>
  constexpr span<element_type, N> last() const {
    return {mData + (mSize - N), N};
  }

  constexpr span<element_type, dynamic_extent> last(index_type count) const {
    return {mData + (mSize - count), count};
  }

  template<std::size_t Offset, std::size_t Count = dynamic_extent>
  auto subspan() const
      -> span<element_type, Count != dynamic_extent
                                ? Count
                                : (extent != dynamic_extent ? (extent - Offset)
                                                            : dynamic_extent)> {
    return {mData + Offset, Count != dynamic_extent ? Count : mSize - Offset};
  }

  constexpr auto subspan(index_type offset,
                         index_type count = dynamic_extent) const
      -> span<element_type, dynamic_extent> {
    return {mData + offset, count == dynamic_extent ? mSize - offset : count};
  }
};

template<class T, std::size_t Extent>
auto as_bytes(span<T, Extent> s) noexcept
    -> span<const std::byte,
            Extent == dynamic_extent ? dynamic_extent : Extent * sizeof(T)> {
  return {reinterpret_cast<const std::byte *>(s.data()), s.size_bytes()};
}

template<class T, std::size_t Extent>
auto as_writable_bytes(span<T, Extent> s) noexcept
    -> span<std::byte,
            Extent == dynamic_extent ? dynamic_extent : Extent * sizeof(T)> {
  return {reinterpret_cast<std::byte *>(s.data()), s.size_bytes()};
}

template<std::size_t I, class T, std::size_t N,
         std::enable_if_t<N != dynamic_extent && (I < N), bool> = true>
constexpr T &get(span<T, N> s) noexcept {
  return s[I];
}

template<class T, std::size_t N>
span(T (&)[N])->span<T, N>;

template<class T, std::size_t N>
span(std::array<T, N> &)->span<T, N>;

template<class T, std::size_t N>
span(const std::array<T, N> &)->span<const T, N>;

template<class Container>
span(Container &)->span<typename Container::value_type>;

template<class Container>
span(const Container &)->span<const typename Container::value_type>;

}  // namespace nostdx

namespace std {

template<class T, size_t N>
struct tuple_size<nostdx::span<T, N>> : integral_constant<size_t, N> {};

template<class T>
struct tuple_size<nostdx::span<T, nostdx::dynamic_extent>>;  // Undefined

template<size_t I, class T, size_t N>
struct tuple_element<I, nostdx::span<T, N>> {
  static_assert(N != nostdx::dynamic_extent && I < N);
  using type = T;
};

}  // namespace std

#endif
