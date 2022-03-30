#pragma once

#include <cassert>
#include <future>
#include <iostream>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

#include "optional.hpp"
#include "traits.hpp"

template <typename R, typename... Args>
class Task {
 public:
  Task() = default;
  explicit Task(std::function<R(Args...)> f)
      : task_(f), arg_(std::optional<Args...>()) {}

  Task(std::function<R(Args...)> f, Args... arg) : task_(f), arg_(arg...) {}

  auto get_result(auto arg) const {
    task_(arg);
    return task_.get_future().get();
  }

  auto get_result() const {
    assert(arg_.has_value());
    task_(arg_.value());
    return task_.get_future().get();
  }

 private:
  std::optional<Args...> arg_;
  mutable std::packaged_task<R(Args...)> task_;
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

template <typename T>
auto mbind(T task, auto next_task) {
  return next_task(task.get_result());
}

std::function id = [](double x) { return x; };
auto id_task(double x) { return make_task(id, x); }

template <typename Ret, typename... Args>
auto operator|(Task<Ret, Args...> task, auto next_task) {
  if constexpr (is_optional_v<Ret>) {
    auto result = task.get_result();
    if (result.isValid()) {
      return next_task(result.value());
    } else {
      std::function stub = [](Args... x) { return nothing<Args...>(); };
      using rtype =
          std::remove_reference_t<std::remove_cv_t<decltype(result.value())>>;
      return make_task(stub, rtype());
    }
  } else {
    return next_task(task.get_result());
  }
}

template <typename F, typename T>
auto transform(F f, T&& task) {
  auto task_ret = task.get_result();
  return make_task(f, task_ret);
}
