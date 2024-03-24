#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

void func() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main() {
    std::jthread t(func);
    std::cout << "Thread ID: " << t.get_id() << std::endl;
    return 0;
}
