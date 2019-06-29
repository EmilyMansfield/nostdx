#ifndef EM_NOSTDX_FUNCTIONAL_HPP
#define EM_NOSTDX_FUNCTIONAL_HPP

namespace nostdx {

/// Helper type for std::visit. Let's you write (for example)
/// ```cpp
/// std::visit(overloaded{
///     [](int i) { doInt(i); },
///     [](float f) { doFloat(f); },
///     [](auto x) { doFallback(x); }
/// }, var);
/// ```
/// instead of messing around with `if constexpr`.
/// See p0051.
template<class ...Ts>
struct overloaded : Ts ... {
  using Ts::operator()...;
};

template<class ...Ts> overloaded(Ts...) -> overloaded<Ts...>;

} // namespace nostdx

#endif
