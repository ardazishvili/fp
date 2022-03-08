#pragma once

#include <type_traits>

template <typename A>
class Optional {
 public:
  Optional() : _is_valid(false) {}
  explicit Optional(A a) : _value(a), _is_valid(true) {}
  bool isValid() const { return _is_valid; }
  A value() const { return _value; }

 private:
  A _value;
  bool _is_valid;
};

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<Optional<T>> : std::true_type {};

template <typename T>
Optional<T> identity1(T t) {
  return Optional<T>(t);
}

template <typename T>
Optional<T> make_optional(T t) {
  return Optional<T>();
}

template <typename Ret, typename Arg,
          typename = std::enable_if_t<is_optional<Ret>::value>>
auto compose(Ret (*f)(Arg)) {
  return [f](auto a) { return f(a); };
}

template <typename Ret, typename Arg,
          typename = std::enable_if_t<is_optional<Ret>::value>, typename... Ts>
auto compose(Ret (*f)(Arg), Ts... ts) {
  return [f, ts...](auto a) {
    auto b = f(a);
    if (!b.isValid()) {
      return make_optional(a);
    }

    return compose(ts...)(b.value());
  };
}

template <typename T>
auto mbind_optional(Optional<T> o, auto f) {
  if (!o.isValid()) {
    return Optional<T>();
  }

  return f(o.value());
}

template <typename T>
auto operator|(Optional<T> o, auto f) {
  if (!o.isValid()) {
    return Optional<T>();
  }

  return f(o.value());
}
