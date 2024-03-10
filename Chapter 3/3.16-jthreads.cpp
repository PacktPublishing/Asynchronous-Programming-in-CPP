#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

class JthreadWrapper {
   public:
    JthreadWrapper(const std::function<void(const std::string&)>& func, const std::string& str)
        : t(func, str), name(str) {}

    ~JthreadWrapper() { std::cout << "Thread " << name << " being destroyed" << std::endl; }

   private:
    std::jthread t;
    std::string name;
};

void func(const std::string& name) {
    std::cout << "Thread " << name << " starting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread " << name << " finishing..." << std::endl;
}

int main() {
    JthreadWrapper t1(func, "t1");
    JthreadWrapper t2(func, "t2");
    JthreadWrapper t3(func, "t3");

    std::this_thread::sleep_for(std::chrono::seconds(4));

    // t1, t2, t3 will be destroyed when main exits
    std::cout << "Main thread exiting..." << std::endl;

    return 0;
}
