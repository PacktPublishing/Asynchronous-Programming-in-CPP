#include <exception>
#include <future>
#include <iostream>
#include <stdexcept>
#include <string>

void print_exceptions(const std::exception& e, int level = 1) {
    auto indent = std::string(2 * level, ' '); 
    std::cerr << indent << e.what() << '\n';
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& nestedException) {
        print_exceptions(nestedException, level + 1);
    } catch (...) { }
}

void func_throwing() {
    // Asynchronous function throwing an exception.
    throw std::runtime_error("Exception in func_throwing.");
}

int main() {
    auto fut = std::async([]() {
        try {
            func_throwing();
        } catch (...) {
            // Rethrow any catched exception.
            std::throw_with_nested(std::runtime_error("Exception in async task."));
        }
    });

    // Main thread: Catch exceptions and print them.
    try {
        fut.get();
    } catch (const std::exception& e) {
        std::cerr << "Caught exceptions:\n";
        print_exceptions(e);
    }

    return 0;
}
