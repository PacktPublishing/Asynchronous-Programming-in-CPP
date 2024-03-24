#include <chrono>
#include <functional>
#include <iostream>
#include <ostream>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;
using namespace std::chrono;

template<typename Duration>
class Timer {
   public:
    typedef std::function<void(void)> Callback;

    Timer(const Duration interval, const Callback& callback) {
        auto value = duration_cast<milliseconds>(interval);
        sync_cout << "Timer: Starting with interval of " << value << std::endl;

        t = std::jthread([&](std::stop_token stop_token) {
            while (!stop_token.stop_requested()) {
                sync_cout << "Timer: Running callback " << val.load() << " ..." << std::endl;
                val++;
                callback();

                sync_cout << "Timer: Sleeping..." << std::endl;
                std::this_thread::sleep_for(interval);
            }
            sync_cout << "Timer: Exit" << std::endl;
        });
    }

    void stop() {
        t.request_stop();
    }

   private:
    std::jthread t;
    std::atomic_int32_t val{0};
};

int main(void) {
    // Create timer executing callback function every second
    sync_cout << "Main: Create timer" << std::endl;
    Timer timer(1s, [&]() {
        sync_cout << "Callback: Running..." << std::endl;
    });

    // Wait main thread for 3 seconds
    std::this_thread::sleep_for(3s);

    // Stop timer
    sync_cout << "Main thread: Stop timer" << std::endl;
    timer.stop();

    // Wait main thread for 500ms while timer stops
    std::this_thread::sleep_for(500ms);
    sync_cout << "Main thread: Exit" << std::endl;
    return 0;
}
