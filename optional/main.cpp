#include <iostream>
#include <optional>

int main() {
  int i = 100;
  std::optional<int> opt = std::make_optional(i);

  std::cout << i << " " << (*opt) << '\n';

  i = 200;

  std::cout << i << " " << (*opt) << '\n';

  return 0;
}