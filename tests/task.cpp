#include "task.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <future>
#include <type_traits>

std::function square = [](double x) { return ::sqrt(x); };

std::function multiply2 = [](double x) { return x * 2; };

std::function iden = [](double x) { return x; };

TEST(TaskTest, IdentityInit) {
  auto value = make_task(iden, 42.0).get_result();
  ASSERT_EQ(value, 42.0);
}

TEST(TaskTest, IdentityNoInit) {
  auto task = make_task(iden);
  ASSERT_EQ(task.get_result(42.0), 42.0);
}

TEST(TaskTest, Compose) {
  auto value = compose(make_task(multiply2), make_task(square))(8);
  ASSERT_EQ(value, 4);
}
