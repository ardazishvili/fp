#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename A>
using Writer = std::pair<A, std::string>;

template <typename T>
struct is_writer : std::false_type {};

template <typename T>
struct is_writer<Writer<T>> : std::true_type {};

template <typename A>
auto identity(A a) {
  return std::make_pair(a, std::string());
}

auto compose(auto a) { return a; }

template <typename... Ts>
auto compose(auto f1, Ts... ts) {
  return [f1, ts...](auto a) {
    auto [b, log1] = f1(a);
    auto p = compose(ts...)(b);
    return std::make_pair(p.first, log1 + p.second);
  };
}

template <typename T>
Writer<T> mbind_writer(Writer<T> w, auto f) {
  auto r = f(w.first);
  return std::pair(r.first, w.second + r.second);
}

template <typename T>
Writer<T> operator|(Writer<T> w, auto f) {
  auto r = f(w.first);
  return std::pair(r.first, w.second + r.second);
}
