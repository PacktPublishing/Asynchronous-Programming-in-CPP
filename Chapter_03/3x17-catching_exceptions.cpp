#include <atomic>
#include <chrono>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

std::exception_ptr captured_exception;
std::mutex mtx;

void func() {
    try {
        std::this_thread::sleep_for(1s);
        throw std::runtime_error("Error in func used within thread");
    } catch (...) {
        // Capture exception into an atomic
        std::lock_guard<std::mutex> lock(mtx);
        captured_exception = std::current_exception();
    }
}

int main() {
    std::thread t(func);

    // Check for captured exception periodically
    while (!captured_exception) {
        std::this_thread::sleep_for(250ms);
        std::cout << "In main thread" << std::endl;
    }

    try {
        // Re-throw the exception
        std::rethrow_exception(captured_exception);
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in main thread: " << e.what() << std::endl;
    }

    t.join();
    return 0;
}
