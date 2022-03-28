#pragma once

#include <type_traits>
#include <utility>

#include "task.hpp"

template <typename A>
class Optional {
 public:
  using inner_type = A;
  Optional() : _is_valid(false) {}
  explicit Optional(A&& a) : _value(std::move(a)), _is_valid(true) {}
  bool isValid() const { return _is_valid; }
  const A& value() const { return _value; }

  template <typename T>
  friend bool operator==(const Optional<T>& o1, const Optional<T>& o2);

 private:
  A _value;
  bool _is_valid;
};

template <typename T>
bool operator==(const Optional<T>& o1, const Optional<T>& o2) {
  if (!o1.isValid() && !o2.isValid()) {
    return true;
  }
  if (o1.isValid() && o2.isValid()) {
    return o1.value() == o2.value();
  }
  return false;
}

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<Optional<T>> : std::true_type {};

template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

template <typename T>
Optional<T> identity(T t) {
  return Optional<T>(t);
}

template <typename T>
Optional<T> make_optional(T t) {
  return Optional<T>();
}

template <typename Ret, typename Arg,
          typename = std::enable_if_t<is_optional_v<Ret>>>
auto compose(Ret (*f)(Arg)) {
  return [f](auto a) { return f(a); };
}

template <typename Ret, typename Arg,
          typename = std::enable_if_t<is_optional_v<Ret>>, typename... Ts>
auto compose(Ret (*f)(Arg), Ts... ts) {
  return [f, ts...](auto a) {
    auto b = f(a);
    if (!b.isValid()) {
      return make_optional(a);
    }

    return compose(ts...)(b.value());
  };
}

template <template <typename> class T, typename D,
          typename = std::enable_if_t<is_optional_v<T<D>>>>
auto mbind(T<D> o, auto f) {
  if (!o.isValid()) {
    return T<D>();
  }

  return f(o.value());
}

template <typename T>
auto operator|(Optional<T> o, auto f) {
  if (!o.isValid()) {
    return Optional<T>();
  }

  if constexpr (is_task_v<T>) {
    return f(o.value().get_result());
  } else {
    return f(o.value());
  }
}

template <typename F, template <typename> class T, typename D,
          typename = std::enable_if_t<is_optional_v<T<D>>>>
auto transform(F f, T<D> o) {
  using inner = D;
  using RetType = std::invoke_result<F, D>::type;
  if (!o.isValid()) {
    return Optional<RetType>();
  }
  return Optional(f(o.value()));
}
