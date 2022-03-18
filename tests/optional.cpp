#include "Optional.hpp"

#include <gtest/gtest.h>

#include <cmath>

#include "generic.hpp"

Optional<double> root(double x) {
  return (x >= 0) ? Optional<double>(std::sqrt(x)) : Optional<double>();
}

Optional<double> reciprocal(double x) {
  return (x != 0) ? Optional<double>(1 / x) : Optional<double>();
}

TEST(OptionalTest, Compose) {
  auto r = compose(reciprocal, root, root)(0.0625);
  ASSERT_TRUE(r.isValid());
  ASSERT_EQ(r.value(), 2);
}

TEST(OptionalTest, Mbind) {
  auto r =
      mbind(mbind(mbind(Optional<double>(0.0625), reciprocal), root), root);
  ASSERT_TRUE(r.isValid());
  ASSERT_EQ(r.value(), 2);
}

TEST(OptionalTest, PipeOperator) {
  auto r = Optional<double>(0.0625) | reciprocal | root | root;
  ASSERT_TRUE(r.isValid());
  ASSERT_EQ(r.value(), 2);
}

TEST(OptionalTest, Transform) {
  auto f = [](int a) { return a * 2; };
  ASSERT_EQ(transform(f, Optional(2)), Optional(4));
}

TEST(OptionalTest, Lift) {
  auto f = [](int a) { return a * 2; };
  auto lifted = lift(f);
  ASSERT_EQ(lifted(Optional(2)), Optional(4));
}
