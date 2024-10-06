#include <iostream>

int main() {
  auto arr = new int[10];
  arr[5] = 0;
  std::cout << "Value at position 0 = " << arr[0] << '\n';
  return 0;
}
