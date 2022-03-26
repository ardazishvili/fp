#include "task.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <future>
#include <string>
#include <type_traits>

#include "generic.hpp"

std::function square = [](double x) { return ::sqrt(x); };
std::function multiply2 = [](double x) { return x * 2; };
std::function iden = [](double x) { return x; };

auto square_task(double x) { return make_task(square, x); }
auto multiply2_task(double x) { return make_task(multiply2, x); }
auto identity_task(double x) { return make_task(iden, x); }

TEST(TaskTest, IdentityInit) {
  auto value = make_task(iden, 42.0);
  ASSERT_EQ(value.get_result(), 42);
}

TEST(TaskTest, IdentityNoInit) {
  auto task = make_task(iden);
  ASSERT_EQ(task.get_result(42.0), 42);
}

TEST(TaskTest, Compose) {
  auto value = compose(make_task(multiply2), make_task(square))(8);
  ASSERT_EQ(value, 4);
}

TEST(TaskTest, Mbind) {
  auto value = mbind(mbind(identity_task(8.0), multiply2_task), square_task);
  ASSERT_EQ(value.get_result(), 4);
}

TEST(TaskTest, PipeOperator) {
  auto value = identity_task(8.0) | multiply2_task | square_task;
  ASSERT_EQ(value.get_result(), 4);
}

TEST(TaskTest, Transform) {
  std::function f = [](int a) { return std::to_string(a); };
  std::function multiply10 = [](int a) { return a * 10; };
  auto task = transform(f, make_task(multiply10, 1));
  ASSERT_TRUE(task.get_result() == "10");
}

TEST(TaskTest, Lift) {
  std::function f = [](int a) { return std::to_string(a); };
  auto lifted = lift(f);
  std::function multiply10 = [](int a) { return a * 10; };
  auto task = lifted(make_task(multiply10, 1));
  ASSERT_TRUE(task.get_result() == "10");
}
