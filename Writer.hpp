#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

template <typename A>
using Writer = std::pair<A, std::string>;

template <typename A>
auto identity(A a) {
  return std::make_pair(a, "");
}

auto compose_writer(auto a) { return a; }

template <typename... Ts>
auto compose_writer(auto f1, Ts... ts) {
  return [f1, ts...](auto a) {
    auto [b, log1] = f1(a);
    auto p = compose_writer(ts...)(b);
    return std::make_pair(p.first, log1 + p.second);
  };
}
