#include "writer.hpp"

#include <gtest/gtest.h>

#include <cmath>

#include "generic.hpp"

Writer<bool> is_even(int n) { return std::make_pair(n % 2 == 0, "is_even "); }
Writer<bool> negate(bool b) { return std::make_pair(!b, "Not so! "); }

TEST(WriterTest, Compose) {
  auto [val, log] = compose(is_even, negate, negate)(6);
  ASSERT_TRUE(val);
  ASSERT_EQ(log, "is_even Not so! Not so! ");
}

TEST(WriterTest, Mbind) {
  auto [val, log] = mbind(mbind(mbind(id_writer(6), is_even), negate), negate);
  ASSERT_TRUE(val);
  ASSERT_EQ(log, "is_even Not so! Not so! ");
}

TEST(WriterTest, PipeOperator) {
  auto [val, log] = id_writer(6) | is_even | negate | negate;
  ASSERT_TRUE(val);
  ASSERT_EQ(log, "is_even Not so! Not so! ");
}

TEST(WriterTest, Transform) {
  auto f = [](int a) { return a * 2; };
  ASSERT_EQ(transform(f, std::make_pair(4, std::string())),
            std::make_pair(8, std::string()));
}

TEST(WriterTest, Lift) {
  auto f = [](int a) { return a * 2; };
  auto lifted = lift(f);
  ASSERT_EQ(lifted(std::make_pair(4, std::string())),
            std::make_pair(8, std::string()));
}
