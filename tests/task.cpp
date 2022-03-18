#include "task.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <future>
#include <type_traits>

std::function square = [](double x) { return ::sqrt(x); };

std::function multiply2 = [](double x) { return x * 2; };

TEST(TaskTest, Compose) {
  auto value = compose(make_task(multiply2), make_task(square))(8);
  ASSERT_EQ(value, 4);
}
