#include <chrono>
#include <functional>
#include <future>
#include <iostream>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

int func(int x) {
    std::this_thread::sleep_for(10ms);
    return 2 * x;
}

int main() {
    constexpr unsigned NUM_TASKS = 32;

    // Prints duration from start time.
    auto duration_from = [](auto start) {
        auto dur = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    };

    // Show results vector size and elements.
    auto show_results = [](const std::vector<int>& res) {
        std::cout << "Results [" << res.size() << "] ";
        for (auto r : res) {
            std::cout << r << " ";
        }
        std::cout << '\n';
    };

    // Running several tasks without storing the future.
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_TASKS; i++) {
        std::async(std::launch::async, func, i);
    }
    std::cout << "Discarding futures: " << duration_from(start) << '\n';

    // Reusing a future, discarding previous tasks.
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_TASKS; i++) {
        auto fut = std::async(std::launch::async, func, i);
    }
    std::cout << "In-place futures: " << duration_from(start) << '\n';

    // Reusing a future, discarding previous tasks.
    std::future<int> fut;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_TASKS; i++) {
        fut = std::async(std::launch::async, func, i);
    }
    std::cout << "Reusing future: " << duration_from(start) << '\n';

    // Same as before but waiting for tasks to finish.
    std::vector<int> res;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_TASKS; i++) {
        fut = std::async(std::launch::async, func, i);
        res.push_back(fut.get());
    }
    std::cout << "Reused future and storing results: " << duration_from(start) << '\n';
    show_results(res);

    // Running several tasks storing a future per task in a vector and waiting for futures afterwards.
    std::vector<std::future<int>> futsVec;
    res.clear();
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_TASKS; i++) {
        futsVec.emplace_back(std::async(std::launch::async, func, i));
    }
    for (int i = 0; i < NUM_TASKS; i++) {
        res.push_back( futsVec[i].get() );
    }
    std::cout << "Futures vector and storing results: " << duration_from(start) << '\n';
    show_results(res);

    return 0;
}
