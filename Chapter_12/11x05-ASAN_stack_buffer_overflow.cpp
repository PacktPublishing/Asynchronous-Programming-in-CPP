#include <iostream>

int main() {
  int arr[100];
  arr[1] = 0;
  auto val = arr[100];
  std::cout << "val = " << val << '\n';
  return 0;
}
