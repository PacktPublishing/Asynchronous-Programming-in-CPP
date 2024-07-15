#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

constexpr int NUM_ITEMS{10000};

void process() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 20);

    int sleep_duration = dis(gen);

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));
}

int main() {
    std::atomic<int> processed_items{0};
    std::atomic<float> total_time{0.0f};
    std::atomic<double> average_time{0.0};

    std::cout << "std::atomic<int> is lock-free: " << std::boolalpha << processed_items.is_lock_free() << std::endl;
    std::cout << "std::atomic<int> is always lock-free: " << std::boolalpha << std::atomic<int>::is_always_lock_free
              << std::endl;
    std::cout << "std::atomic<float> is lock-free: " << std::boolalpha << total_time.is_lock_free() << std::endl;
    std::cout << "std::atomic<float> is always lock-free: " << std::boolalpha << std::atomic<float>::is_always_lock_free
              << std::endl;
    std::cout << "std::atomic<double> is lock-free: " << std::boolalpha << average_time.is_lock_free() << std::endl;
    std::cout << "std::atomic<double> is always lock-free: " << std::boolalpha
              << std::atomic<double>::is_always_lock_free << std::endl;

    std::thread worker([&] {
        for (int i = 1; i <= NUM_ITEMS; ++i) {
            auto now = std::chrono::high_resolution_clock::now();
            process();
            auto elapsed = std::chrono::high_resolution_clock::now() - now;
            float elapsed_s = std::chrono::duration<float>(elapsed).count();

            processed_items.fetch_add(1, std::memory_order_relaxed);
            total_time.fetch_add(elapsed_s, std::memory_order_relaxed);
            average_time.store(total_time.load() / processed_items.load(), std::memory_order_relaxed);
        }
    });

    while (true) {
        int items = processed_items.load(std::memory_order_relaxed);
        std::cout << "Progress: " << items << " / " << NUM_ITEMS << std::endl;

        float time = total_time.load(std::memory_order_relaxed);
        std::cout << "Total time: " << time << " sec" << std::endl;

        double average = average_time.load(std::memory_order_relaxed);
        std::cout << "Average time: " << average * 1000 << " ms" << std::endl;

        if (items == NUM_ITEMS) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    worker.join();

    return 0;
}
