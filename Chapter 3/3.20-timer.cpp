#include <thread>
#include <iostream>
#include <chrono>
#include <functional>
#include <syncstream>
#include <ostream>

namespace {
    std::osyncstream synced_out(std::cout);
}

using namespace std::chrono_literals;

class Timer
{
public:
    typedef std::chrono::milliseconds Interval;
    typedef std::function<void(void)> Callback;

    Timer(const Interval &interval, const Callback &callback) {
        synced_out << "Timer: Starting with interval " << interval << std::endl;
        t = std::jthread([&]() {
            while (running.load() == true) {
                synced_out << "Timer: Running callback " << val.load() << std::endl;
                val++;
                callback();
                synced_out.flush();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));                
            }
            synced_out << "Timer: Exit" << std::endl;
        });
    }

    void stop() {
        running.store(false);
    }

private:
    std::jthread t;
    std::atomic_bool running{true};        
    std::atomic_int32_t val{0};
};

int main(void)
{
    // Create timer executing callback function every 500ms
    synced_out << "Main: Create timer" << std::endl;
    Timer timer(std::chrono::milliseconds(500), [&]() {
        synced_out << "Callback: Running..." << std::endl;
    });

    // Use secondary thread to stop timer after 2 seconds
    std::jthread t([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        synced_out << "Thread2: Stopping timer" << std::endl;
        timer.stop();
        synced_out << "Thread2: Exit thread" << std::endl;
    });

    // Wait main thread for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));
    synced_out << "Main thread: Exit" << std::endl;
    return 0;
}
