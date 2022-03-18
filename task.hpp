#pragma once

#include <future>

template <typename T>
auto compose(T&& t) {
  return [&t](auto a) {
    t(a);
    return t.get_future().get();
  };
}

template <typename T, typename... Args>
auto compose(T&& t, Args&&... args) {
  return [&t, ... args = std::forward<Args>(args)](auto a) mutable {
    t(a);
    auto ret = t.get_future().get();
    return compose(args...)(ret);
  };
}

template <typename R, typename... Args>
auto make_task(std::function<R(Args...)> f) {
  std::packaged_task<R(Args...)> task2(f);
  return task2;
}
