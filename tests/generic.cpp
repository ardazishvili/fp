#include "generic.hpp"

#include <gtest/gtest.h>

#include <cmath>

auto print(int a, bool b, std::string s) {
  return std::to_string(a) + " " + std::to_string(b) + " " + s;
}

TEST(GenericTest, Curry) {
  auto curried = curry(print);
  ASSERT_EQ(curried(1, true, "s"), "1 1 s");
  ASSERT_EQ(curried(1)(true, "s"), "1 1 s");
  ASSERT_EQ(curried(1)(true)("s"), "1 1 s");
}
