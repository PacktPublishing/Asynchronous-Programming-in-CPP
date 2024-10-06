#include <iostream>
#include <thread>

class MyObj {};

static MyObj *obj = nullptr;

void init_object() { 
  if (!obj) {
    obj = new MyObj(); 
  }
}

void func1() {
  init_object();
}

void func2() {
  init_object();
}

int main() {
  std::thread t1(func1);
  std::thread t2(func2);

  t1.join();
  t2.join();  
  return 0;
}
