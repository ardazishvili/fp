#pragma once

#include <type_traits>

template <typename T>
class Optional;

template <typename R = void, typename... Args>
class Task;

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<Optional<T>> : std::true_type {};

template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

template <typename... Args>
struct is_task : std::false_type {};

template <typename... Args>
struct is_task<Task<Args...>> : std::true_type {};

template <typename T>
constexpr bool is_task_v = is_task<T>::value;

struct Dummy {};
