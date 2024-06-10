#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

namespace {
int result = 0;
std::mutex mtx;
};

void func(int& result) {
    // Simulate some work and return random value in range [1,10]
    std::this_thread::sleep_for(1s);
    result = 1 + (rand() % 10);
}

void funcWithMutex() {
    // Simulating some computation
    std::this_thread::sleep_for(1s);
    int localVar = 1 + (rand() % 10);

    // Lock the mutex before updating the shared variable
    std::lock_guard<std::mutex> lock(mtx);
    result = localVar;
}

int main() {
    // Get result value by reference
    std::thread t1(func, std::ref(result));
    t1.join();
    sync_cout << "T1 result: " << result << std::endl;

    // Get result value by lambda capture
    std::thread t2([&]() { func(result); });
    t2.join();
    sync_cout << "T2 result: " << result << std::endl;

    // Get result value by shared variable and mutex
    std::thread t3(funcWithMutex);
    t3.join();
    sync_cout << "T3 result: " << result << std::endl;

    return 0;
}
