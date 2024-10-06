#include <thread>

int globalVar{0};

void increase() {
  globalVar++;
}

void decrease() {
  globalVar--;  
}

int main() {
  std::thread t1(increase);
  std::thread t2(decrease);

  t1.join();
  t2.join();
  return 0;
}
