#include <iostream>
#include <thread>

int main() {
    std::thread t1;
    std::cout << "T1 joinable? " << t1.joinable() << std::endl;

    return 0;
}
