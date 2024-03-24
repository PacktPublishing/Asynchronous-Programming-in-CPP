#include <chrono>
#include <functional>
#include <iostream>
#include <ostream>
#include <syncstream>
#include <thread>

namespace {
std::osyncstream sync_out(std::cout);
}

using namespace std::chrono_literals;

class Timer {
   public:
    typedef std::chrono::milliseconds Interval;
    typedef std::function<void(void)> Callback;

    Timer(const Interval &interval, const Callback &callback) {
        sync_out << "Timer: Starting with interval " << interval << std::endl;
        t = std::jthread([&]() {
            while (running.load() == true) {
                sync_out << "Timer: Running callback " << val.load() << std::endl;
                val++;
                callback();
                sync_out.flush();
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            }
            sync_out << "Timer: Exit" << std::endl;
        });
    }

    void stop() { running.store(false); }

   private:
    std::jthread t;
    std::atomic_bool running{true};
    std::atomic_int32_t val{0};
};

int main(void) {
    // Create timer executing callback function every 500ms
    sync_out << "Main: Create timer" << std::endl;
    Timer timer(std::chrono::milliseconds(500), [&]() { sync_out << "Callback: Running..." << std::endl; });

    // Use secondary thread to stop timer after 2 seconds
    std::jthread t([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        sync_out << "Thread2: Stopping timer" << std::endl;
        timer.stop();
        sync_out << "Thread2: Exit thread" << std::endl;
    });

    // Wait main thread for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));
    sync_out << "Main thread: Exit" << std::endl;
    return 0;
}
