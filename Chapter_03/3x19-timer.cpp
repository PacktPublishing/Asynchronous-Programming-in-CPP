#include <chrono>
#include <functional>
#include <iostream>
#include <ostream>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

class Timer {
   public:
    typedef std::chrono::milliseconds Interval;
    typedef std::function<void(void)> Callback;

    Timer(const Interval &interval, const Callback &callback) {
        sync_cout << "Timer: Starting with interval " << interval << std::endl;
        t = std::jthread([&]() {
            while (running.load() == true) {
                sync_cout << "Timer: Running callback " << val.load() << std::endl;
                val++;
                callback();
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            }
            sync_cout << "Timer: Exit" << std::endl;
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
    sync_cout << "Main: Create timer" << std::endl;
    Timer timer(std::chrono::milliseconds(500), [&]() { 
        sync_cout << "Callback: Running..." << std::endl; 
    });

    // Use secondary thread to stop timer after 2 seconds
    std::jthread t([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        sync_cout << "Thread2: Stopping timer" << std::endl;
        timer.stop();
        sync_cout << "Thread2: Exit thread" << std::endl;
    });

    // Wait main thread for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));
    sync_cout << "Main thread: Exit" << std::endl;
    return 0;
}
