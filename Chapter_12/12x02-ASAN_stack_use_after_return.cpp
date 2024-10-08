// By default, AddressSanitizer does not try to detect
// stack-use-after-return bugs.
// Need to run with ASAN_OPTIONS=detect_stack_use_after_return=1

#include <iostream>

int *ptr = nullptr;

__attribute__((noinline))
void func() {
  int local[100];
  // Danger: Pointer to local variable
  ptr = &local[0];
}

int main() {
  func();
  std::cout << "Pointer value: " << *ptr << '\n';
  return 0;
}
