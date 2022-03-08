#include <cassert>
#include <cmath>
#include <string>
#include <type_traits>

#include "Optional.hpp"
#include "Writer.hpp"
#include "helpers.hpp"

Writer<bool> is_even(int n) { return std::make_pair(n % 2 == 0, "is_even "); }
Writer<bool> negate(bool b) { return std::make_pair(!b, "Not so! "); }

Optional<double> root(double x) {
  return (x >= 0) ? Optional<double>(std::sqrt(x)) : Optional<double>();
}

Optional<double> reciprocal(double x) {
  return (x != 0) ? Optional<double>(1 / x) : Optional<double>();
}

auto print(int a, bool b, std::string s) {
  return std::to_string(a) + " " + std::to_string(b) + " " + s;
}

auto foo(int a) { return a * 1; }
struct S {
  double operator()(char, int&);
  float operator()(int) { return 1.0; }
};

int main() {
  {
    auto [val, log] = compose(is_even, negate, negate)(6);
    assert(val == true);
    assert(log == "is_even Not so! Not so! ");
  }
  {
    auto r = compose(reciprocal, root, root)(0.0625);
    assert(r.isValid());
    assert(r.value() == 2);
  }
  {
    auto r = mbind_optional(
        mbind_optional(mbind_optional(Optional<double>(0.0625), reciprocal),
                       root),
        root);
    assert(r.isValid());
    assert(r.value() == 2);
  }
  {
    auto r = Optional<double>(0.0625) | reciprocal | root | root;
    assert(r.isValid());
    assert(r.value() == 2);
  }
  {
    auto [val, log] = mbind_writer(
        mbind_writer(mbind_writer(identity(6), is_even), negate), negate);
    assert(val == true);
    assert(log == "is_even Not so! Not so! ");
  }
  {
    auto [val, log] = identity(6) | is_even | negate | negate;
    assert(val == true);
    assert(log == "is_even Not so! Not so! ");
  }
  {
    auto curried = curry(print);
    assert(curried(1, true, "s") == "1 1 s");
    assert(curried(1)(true, "s") == "1 1 s");
    assert(curried(1)(true)("s") == "1 1 s");
  }
  {
    auto f = [](int a) { return a * 2; };
    assert(transform(f, Optional(2)) == Optional(4));
  }

  {
    auto f = [](int a) { return a * 2; };
    auto lifted = lift_optional(f);
    assert(lifted(Optional(2)) == Optional(4));
  }

  return 0;
}
