#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

constexpr int NUM_ITEMS{100000};

int main() {
    using namespace std::chrono_literals;

    std::atomic<int> progress{0};

    std::thread worker([&progress] {
        for (int i = 1; i <= NUM_ITEMS; ++i) {
            progress.store(i, std::memory_order_relaxed);
            std::this_thread::sleep_for(1ms);
        }
    });

    while (true) {
        int processed_items = progress.load(std::memory_order_relaxed);
        std::cout << "Progress: " << processed_items << " / " << NUM_ITEMS << std::endl;
        if (processed_items == NUM_ITEMS) {
            break;
        }
        std::this_thread::sleep_for(10s);
    }

    worker.join();

    return 0;
}