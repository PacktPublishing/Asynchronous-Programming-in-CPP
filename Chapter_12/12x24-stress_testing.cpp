#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<int> counter(0);
const std::size_t total_runs = 100;

void asyncIncrement() {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
    counter.fetch_add(1);
}

TEST(AsyncTest, StressTest) {
    std::vector<std::thread> threads;

    for (std::size_t i = 0; i < total_runs; ++i) {
        threads.emplace_back(asyncIncrement);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    EXPECT_EQ(counter, total_runs);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
