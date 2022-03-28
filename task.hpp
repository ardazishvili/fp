#pragma once

#include <cassert>
#include <future>
#include <memory>
#include <optional>
#include <type_traits>

template <typename R = void, typename... Args>
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

template <typename... Args>
struct is_task : std::false_type {};

template <typename... Args>
struct is_task<Task<Args...>> : std::true_type {};

template <typename T>
constexpr bool is_task_v = is_task<T>::value;

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

template <typename... Args>
auto operator|(Task<Args...> task, auto next_task) {
  return next_task(task.get_result());
}

template <typename F, typename T>
auto transform(F f, T&& task) {
  auto task_ret = task.get_result();
  return make_task(f, task_ret);
}
