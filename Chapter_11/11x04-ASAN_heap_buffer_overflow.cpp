#include <iostream>

int main() {
  auto arr = new int[100];
  arr[0] = 0;
  int res = arr[100];
  std::cout << "res = " << res << '\n';
  delete[] arr;
  return 0;
}
