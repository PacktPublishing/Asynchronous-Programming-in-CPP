#include <chrono>
#include <iostream>
#include <syncstream>
#include <thread>

namespace {
std::osyncstream sync_out(std::cout);
int timeout = 3;
}

void daemonThread() {
    sync_out << "Daemon thread starting..." << std::endl;
    while (timeout-- > 0) {
        sync_out << "Daemon thread is running..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    sync_out << "Daemon thread exiting..." << std::endl;
}

int main() {
    std::thread t(daemonThread);
    t.detach();

    std::this_thread::sleep_for(std::chrono::seconds(timeout + 1));

    sync_out << "Main thread exiting..." << std::endl;
    return 0;
}
