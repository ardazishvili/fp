#include <gtest/gtest.h>

#include <cmath>

#include "Optional.hpp"
#include "task.hpp"

std::function square = [](double x) { return ::sqrt(x); };
std::function reciprocal = [](double x) { return 1 / x; };
std::function iden = [](double x) { return x; };

auto square_task(double x) { return make_task(square, x); }
auto identity_task(double x) { return make_task(iden, x); }

template <typename R, typename... Args>
auto task_positive_optional(std::function<R(Args...)> f, double x) {
  return x >= 0 ? Optional(make_task(f, x)) : Optional<Task<R, Args...>>();
}

auto task_square_optional(double x) {
  return task_positive_optional(square, x);
}

template <typename R, typename... Args>
auto task_nonzero_optional(std::function<R(Args...)> f, double x) {
  return x != 0 ? Optional(make_task(f, x)) : Optional<Task<R, Args...>>();
}

auto task_reciprocal_optional(double x) {
  return task_nonzero_optional(reciprocal, x);
}

TEST(TaskOptionalTest, PipingTwoValid) {
  {
    auto opt = Optional(identity_task(0.0625)) | task_reciprocal_optional;
    ASSERT_TRUE(opt.isValid());
    ASSERT_EQ(opt.value().get_result(), 16);
  }

  {
    auto opt = Optional(identity_task(16)) | task_square_optional;
    ASSERT_TRUE(opt.isValid());
    ASSERT_EQ(opt.value().get_result(), 4);
  }
}

TEST(TaskOptionalTest, PipingTwoInvalid) {
  {
    auto opt = Optional(identity_task(0)) | task_reciprocal_optional;
    ASSERT_FALSE(opt.isValid());
  }

  {
    auto opt = Optional(identity_task(-16)) | task_square_optional;
    ASSERT_FALSE(opt.isValid());
  }
}

TEST(TaskOptionalTest, PipingThreeValid) {
  auto opt = Optional(identity_task(0.0625)) | task_reciprocal_optional |
             task_square_optional;
  ASSERT_TRUE(opt.isValid());
  ASSERT_EQ(opt.value().get_result(), 4);
}

TEST(TaskOptionalTest, PipingThreeInvalid) {
  {
    auto opt = Optional(identity_task(0)) | task_reciprocal_optional |
               task_square_optional;
    ASSERT_FALSE(opt.isValid());
  }
  {
    auto opt = Optional(identity_task(-0.0625)) | task_reciprocal_optional |
               task_square_optional;
    ASSERT_FALSE(opt.isValid());
  }
}
