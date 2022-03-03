#include <cmath>

#include "Optional.hpp"
#include "Writer.hpp"

Writer<bool> isEven(int n) { return std::make_pair(n % 2 == 0, "isEven "); }
Writer<bool> negate(bool b) { return std::make_pair(!b, "Not so! "); }

Optional<double> safe_root(double x) {
  return (x >= 0) ? Optional<double>(std::sqrt(x)) : Optional<double>();
}
Optional<double> safe_reciprocal(double x) {
  return (x != 0) ? Optional<double>(1 / x) : Optional<double>();
}

int main() {
  {
    auto [val, log] = compose_writer(isEven, negate, negate)(6);
    std::cout << "Value: " << val << std::endl;
    std::cout << "Log: " << log << std::endl;
  }
  {
    auto r = compose_optional(safe_reciprocal, safe_root, safe_root)(0.25);
    std::cout << "isValid: " << r.isValid() << std::endl;
    std::cout << "value: " << r.value() << std::endl;
  }
  return 0;
}
