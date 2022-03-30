#include <gtest/gtest.h>

#include <cmath>

#include "optional.hpp"
#include "task.hpp"

Optional<double> root_f(double x) {
  return (x >= 0) ? Optional<double>(std::sqrt(x)) : nothing<double>();
}

Optional<double> reciprocal_f(double x) {
  return (x != 0) ? Optional<double>(1 / x) : nothing<double>();
}

std::function reciprocal = [](double x) {
  return (x != 0) ? Optional<double>(1 / x) : nothing<double>();
};

std::function root = [](double x) {
  return (x >= 0) ? Optional<double>(std::sqrt(x)) : nothing<double>();
};

std::function id_opt = [](double x) { return Optional(std::move(x)); };

auto id_opt_task(double x) { return make_task(id_opt, x); }
auto reciprocal_task(double x) { return make_task(reciprocal, x); }
auto root_task(double x) { return make_task(root, x); }

TEST(OptionalTaskTest, PipingTwoValid) {
  {
    auto task = id_opt_task(42.0);
    auto result = task.get_result();
    ASSERT_TRUE(result.isValid());
    ASSERT_EQ(result.value(), 42.0);
  }
  {
    auto task = id_opt_task(42.0) | id_opt_task;
    auto result = task.get_result();
    ASSERT_TRUE(result.isValid());
    ASSERT_EQ(result.value(), 42.0);
  }
  {
    auto task = id_opt_task(2.0) | reciprocal_task;
    auto result = task.get_result();
    ASSERT_TRUE(result.isValid());
    ASSERT_EQ(result.value(), 0.5);
  }
  {
    auto task = reciprocal_task(2.0) | id_opt_task;
    auto result = task.get_result();
    ASSERT_TRUE(result.isValid());
    ASSERT_EQ(result.value(), 0.5);
  }
}

TEST(OptionalTaskTest, PipingTwoInvalid) {
  {
    auto task = id_opt_task(0) | reciprocal_task;
    auto result = task.get_result();
    ASSERT_FALSE(result.isValid());
  }
  {
    auto task = reciprocal_task(0) | id_opt_task;
    auto result = task.get_result();
    ASSERT_FALSE(result.isValid());
  }
}

TEST(OptionalTaskTest, PipingManyInvalid) {
  {
    auto task = id_opt_task(0) | reciprocal_task | id_opt_task;
    auto result = task.get_result();
    ASSERT_FALSE(result.isValid());
  }
  {
    auto task = reciprocal_task(0) | id_opt_task | reciprocal_task;
    auto result = task.get_result();
    ASSERT_FALSE(result.isValid());
  }
  {
    auto task = reciprocal_task(0) | id_opt_task | id_opt_task;
    auto result = task.get_result();
    ASSERT_FALSE(result.isValid());
  }
  {
    auto task = id_opt_task(-2.0) | reciprocal_task | root_task;
    auto result = task.get_result();
    ASSERT_FALSE(result.isValid());
  }
  {
    auto task = reciprocal_task(-2.0) | id_opt_task | root_task;
    auto result = task.get_result();
    ASSERT_FALSE(result.isValid());
  }
  {
    auto task = reciprocal_task(0) | id_opt_task | root_task;
    auto result = task.get_result();
    ASSERT_FALSE(result.isValid());
  }
}
