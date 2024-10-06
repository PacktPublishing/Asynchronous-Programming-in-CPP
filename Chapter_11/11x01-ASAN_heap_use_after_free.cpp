#include <iostream>
#include <memory>

int main() {    
  auto arr = new int[100];
  delete[] arr;  
  std::cout << "arr[0] = " << arr[0] << '\n';
  return 0;
}