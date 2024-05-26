#include <future>
#include <iostream>
#include <syncstream>
#include <thread>
#include <vector>

#define sync_cout std::osyncstream(std::cout)

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::shared_future<int> shared_fut = fut.share();

    std::vector<std::jthread> threads;
    for (int i = 1; i <= 5; ++i) {
        // Each thread will get the result of the shared_future.
        threads.emplace_back(
            [shared_fut, i]() { sync_cout << "Thread " << i << ": Result = " << shared_fut.get() << std::endl; });
    }

    // At this point, all threads are running and waiting for the result.
    // Here we send the same result to all of them.
    prom.set_value(5);

    return 0;
}
