#include <chrono>
#include <iostream>
#include <thread>

namespace {
int timeout = 3;
}

void daemonThread() {
    std::cout << "Daemon thread starting..." << std::endl;
    while (timeout-- > 0) {
        std::cout << "Daemon thread is running..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Daemon thread exiting..." << std::endl;
}

int main() {
    std::thread t(daemonThread);
    t.detach();

    std::this_thread::sleep_for(std::chrono::seconds(timeout + 1));

    std::cout << "Main thread exiting..." << std::endl;
    return 0;
}
