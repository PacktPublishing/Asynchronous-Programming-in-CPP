#include <chrono>
#include <iostream>
#include <thread>

void func() { 
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
}

int main() {
    std::thread t1;
    std::cout << "Is t1 joinable? " << t1.joinable() << std::endl;

    std::thread t2(func);
    t1.swap(t2);
    std::cout << "Is t1 joinable? " << t1.joinable() << std::endl;
    std::cout << "Is t2 joinable? " << t2.joinable() << std::endl;

    t1.join();
    std::cout << "Is t1 joinable? " << t1.joinable() << std::endl;

    return 0;
}
