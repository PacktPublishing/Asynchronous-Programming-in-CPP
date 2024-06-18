#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <thread>
#include <vector>
#include <syncstream>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

int compute(unsigned taskId, int x, int y) {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 200));  // Simulate long computation
    sync_cout << "Running task " << taskId << '\n';
    return std::pow(x, y);
}

int main() {
    // Launch compute asynchronously
    std::vector<std::future<int>> futVec;
    for (int i = 0; i <= 10; i++)
        futVec.emplace_back(std::async(compute, i+1, 2, i));

    // Do other work in the main thread
    sync_cout << "Waiting in main thread\n";
    std::this_thread::sleep_for(1s);

    // Waiting for results and storing in output vector
    std::vector<int> results;
    for (auto& fut : futVec)
        results.push_back(fut.get());

    // Showing results
    for (auto& res : results)
        std::cout << res << ' ';
    std::cout << std::endl;
    return 0;
}
