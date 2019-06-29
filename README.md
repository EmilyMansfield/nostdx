# `nostdx`

Yet another small miscellaneous header-only utility library.

Intended to contain implementations of some C++ proposals that I've needed but
that haven't been merged into the standard yet, or are in `std::experimental`
but aren't implemented by all compilers, or haven't been accepted etc.

- `functional.hpp`
  - `overload` ([P0051](wg21.link/p0051), [cppreference](https://en.cppreference.com/w/cpp/utility/variant/visit))
- `propagate_const.hpp`
  - `propagate_const` ([N4562](wg21.link/n4562), [cppreference](https://en.cppreference.com/w/cpp/experimental/propagate_const))
- `type_traits.hpp`
  - `type_identity` (C++20, [cppreference](https://en.cppreference.com/w/cpp/types/type_identity))

Any modern compiler supporting C++17 should be sufficient.
