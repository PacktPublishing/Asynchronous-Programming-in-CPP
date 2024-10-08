#include <map>
#include <thread>

std::map<int,int> m;

void Thread1() {
  m[123] = 1;
}

void Thread2() {
  m[345] = 0;
}

int main() {
  std::jthread t1(Thread1);
  std::jthread t2(Thread2);  
  return 0;
}
