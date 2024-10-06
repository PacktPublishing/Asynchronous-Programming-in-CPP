#include <iostream>
#include <thread>
#include <map>
#include <string>

typedef std::map<std::string, std::string> map_t;

void *func(void *p) {
  map_t& m = *static_cast<map_t*>(p);
  m["foo"] = "bar";
  return 0;
}

int main() {
  map_t m;
  std::thread t(func, &m);
  std::cout << "foo = " << m["foo"] << '\n';
  t.join();
  return 0;
}
