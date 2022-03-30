#include "optional.hpp"

#include <gtest/gtest.h>

#include <cmath>

#include "generic.hpp"

Optional<double> root(double x) {
  return (x >= 0) ? Optional<double>(std::sqrt(x)) : nothing<double>();
}

Optional<double> reciprocal(double x) {
  return (x != 0) ? Optional<double>(1 / x) : nothing<double>();
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

auto id_f(auto t) { return Optional(std::move(t)); }

TEST(OptionalTest, PipingIdentityBasic) {
  {
    auto r = Optional<int>(2) | id_optional;
    ASSERT_TRUE(r.isValid());
    ASSERT_EQ(r.value(), 2);
  }
  {
    auto r = id_optional(2.0) | reciprocal;
    ASSERT_TRUE(r.isValid());
    ASSERT_EQ(r.value(), 0.5);
  }
}

TEST(OptionalTest, PipingIdentity) {
  {
    auto r = id_optional(2) | id_optional | id_optional;
    ASSERT_TRUE(r.isValid());
    ASSERT_EQ(r.value(), 2);
  }
  {
    auto r = id_optional(2.0) | reciprocal | id_optional;
    ASSERT_TRUE(r.isValid());
    ASSERT_EQ(r.value(), 0.5);
  }
  {
    auto r = id_optional(2.0) | reciprocal | reciprocal;
    ASSERT_TRUE(r.isValid());
    ASSERT_EQ(r.value(), 2.0);
  }
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
