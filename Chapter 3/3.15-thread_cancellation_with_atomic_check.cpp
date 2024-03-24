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

class Counter {
    using Callback = std::function<void(void)>;
   
   public:

    Counter(const Callback &callback) {
        sync_out << "Counter: Starting..." << std::endl;
        t = std::jthread([&]() {
            while (running.load() == true) {
                sync_out << "Counter: Running callback..." << std::endl;
                callback();
                sync_out.flush();
                std::this_thread::sleep_for(std::chrono::milliseconds(1s));
            }
            sync_out << "Counter: Exit" << std::endl;
        });
    }

    void stop() { running.store(false); }

   private:
    std::jthread t;
    std::atomic_bool running{true};
};

int main(void) {
    // Create timer executing callback function every 500ms
    sync_out << "Main: Create Counter" << std::endl;
    Counter counter([&]() { 
        sync_out << "Callback: Running..." << std::endl; 
    });

    // Wait main thread for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Stop counter and wait for 1 second
    counter.stop();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    sync_out << "Main thread: Exit" << std::endl;
    return 0;
}
