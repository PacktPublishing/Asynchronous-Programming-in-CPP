#include <chrono>
#include <iostream>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

namespace {
int timeout = 3;
}

void daemonThread() {
    sync_cout << "Daemon thread starting..." << std::endl;
    while (timeout-- > 0) {
        sync_cout << "Daemon thread is running..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    sync_cout << "Daemon thread exiting..." << std::endl;
}

int main() {
    std::thread t(daemonThread);
    t.detach();

    std::this_thread::sleep_for(std::chrono::seconds(timeout + 1));

    sync_cout << "Main thread exiting..." << std::endl;
    return 0;
}
