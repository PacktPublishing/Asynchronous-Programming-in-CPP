#include <chrono>
#include <future>
#include <iostream>
#include <semaphore>
#include <syncstream>
#include <vector>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

void task(int id, std::counting_semaphore<>& sem) {
    // Acquire the semaphore
    sem.acquire();

    // Simulate some work
    sync_cout << "Running task " << id << "...\n";
    std::this_thread::sleep_for(1s);

    // Release the semaphore
    sem.release();
}

int main() {
    const int total_tasks = 20;
    const int max_concurrent_tasks = std::thread::hardware_concurrency();
    std::counting_semaphore<> sem(max_concurrent_tasks);

    sync_cout << "Allowing only " << max_concurrent_tasks 
              << " concurrent tasks to run " << total_tasks << " tasks.\n";

    std::vector<std::future<void>> futures;
    for (int i = 0; i < total_tasks; ++i) {
        futures.push_back(std::async(std::launch::async, task, i, std::ref(sem)));
    }

    for (auto& fut : futures) {
        fut.get();
    }
    std::cout << "All tasks completed." << std::endl;
    return 0;
}
