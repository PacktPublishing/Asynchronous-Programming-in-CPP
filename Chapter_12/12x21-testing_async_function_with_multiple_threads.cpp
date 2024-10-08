#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <syncstream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

#define sync_cout std::osyncstream(std::cout)

std::condition_variable cv;
std::mutex mtx;

bool ready = false;
std::atomic<unsigned> counter = 0;

const std::size_t num_threads = 5;

void asyncTask(int id) {
    sync_cout << "Thread " << id << ": Starting work..." << std::endl;
    std::this_thread::sleep_for(100ms);
    sync_cout << "Thread " << id << ": Work finished." << std::endl;

    ++counter;
    cv.notify_one();
}

TEST(AsyncTest, TestMultipleThreads) {    
    std::vector<std::jthread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(asyncTask, i + 1);
    }

    {
        std::unique_lock<std::mutex> lock(mtx); 
        cv.wait_for(lock, 150ms, [] { return counter == num_threads; });
        sync_cout << "All threads have finished." << std::endl;
    }

    EXPECT_EQ(counter, num_threads);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
