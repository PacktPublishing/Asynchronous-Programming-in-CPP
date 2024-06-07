#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void func() {
    std::this_thread::sleep_for(1s);
}

int main() {
    std::jthread t(func);
    std::cout << "Thread ID: " << t.get_id() << std::endl;
    return 0;
}
