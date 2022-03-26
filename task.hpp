#pragma once

#include <cassert>
#include <future>
#include <memory>
#include <optional>

template <typename R, typename... Args>
class Task {
 public:
  explicit Task(std::function<R(Args...)> f)
      : task_(f), arg_(std::optional<Args...>()) {}

  Task(std::function<R(Args...)> f, Args... arg) : task_(f), arg_(arg...) {}

  auto get_result(auto arg) {
    task_(arg);
    return task_.get_future().get();
  }

  auto get_result() {
    assert(arg_.has_value());
    task_(arg_.value());
    return task_.get_future().get();
  }

 private:
  std::optional<Args...> arg_;
  std::packaged_task<R(Args...)> task_;
};

template <typename T>
auto compose(T&& t) {
  return [&t](auto a) { return t.get_result(a); };
}

template <typename T, typename... Args>
auto compose(T&& t, Args&&... args) {
  return [&t, ... args = std::forward<Args>(args)](auto a) mutable {
    auto ret = t.get_result(a);
    return compose(args...)(ret);
  };
}

template <typename R, typename... Args>
auto make_task(std::function<R(Args...)> f) {
  return Task(f);
}

template <typename R, typename... Args>
auto make_task(std::function<R(Args...)> f, Args... arg) {
  return Task(f, arg...);
}
