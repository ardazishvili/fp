#pragma once

#include <tuple>

template <typename F>
auto lift(F f) {
  return [f](auto w) { return transform(f, w); };
}

template <typename F, typename... CapturedArgs>
struct curry {
  curry(F f, CapturedArgs... args)
      : f(f), captured(capture_by_copy(std::move(args)...)) {}

  curry(F f, std::tuple<CapturedArgs...> args)
      : f(f), captured(std::move(args)) {}

  template <typename... Args>
  static auto capture_by_copy(Args&&... args) {
    return std::tuple<std::decay_t<Args>...>(std::forward<Args>(args)...);
  }

  template <typename... NewArgs>
  auto operator()(NewArgs&&... args) const {
    auto new_args = capture_by_copy(std::forward<NewArgs>(args)...);
    auto all_args = std::tuple_cat(captured, std::move(new_args));

    if constexpr (std::is_invocable_v<F, CapturedArgs..., NewArgs...>) {
      return std::apply(f, all_args);
    } else {
      return curry<F, CapturedArgs..., NewArgs...>(f, all_args);
    }
  }

 private:
  F f;
  std::tuple<CapturedArgs...> captured;
};
