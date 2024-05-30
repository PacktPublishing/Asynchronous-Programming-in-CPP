#include <chrono>
#include <format>
#include <iostream>
#include <thread>

#include "mutex_queue.h"

void push_pop(std::size_t queue_size, int items, int producer_wait, int consumer_wait) {
    async_prog::queue<int> queue(queue_size);

    std::jthread producer([&] {
        for (int i = 0; i < items; ++i) {
            queue.push(i);
            std::cout << std::format("Producer push {0}\n", i);

            std::this_thread::sleep_for(std::chrono::milliseconds(producer_wait));
        }
    });

    std::jthread consumer([&] {
        for (int i = 0; i < items; ++i) {
            int item{};
            queue.pop(item);
            std::cout << std::format("Consumer pop {0}\n", item);

            std::this_thread::sleep_for(std::chrono::milliseconds(consumer_wait));
        }
    });
}

void try_push_try_pop(std::size_t queue_size, int items, int producer_wait, int consumer_wait) {
    async_prog::queue<int> queue(queue_size);

    std::jthread producer([&] {
        int i{};

        while (i < items) {
            if (queue.try_push(i)) {
                std::cout << std::format("Producer push {0}\n", i);
                std::this_thread::sleep_for(std::chrono::milliseconds(producer_wait));

                ++i;
            }
        }
    });

    std::jthread consumer([&] {
        int i{};

        while (i < items) {
            int item{};

            if (queue.try_pop(item)) {
                std::cout << std::format("Consumer pop {0}\n", item);
                std::this_thread::sleep_for(std::chrono::milliseconds(consumer_wait));

                ++i;
            }
        }
    });
}

int main() {
    push_pop(5, 1000, 1, 5);
    try_push_try_pop(5, 1000, 1, 5);

    return 0;
}