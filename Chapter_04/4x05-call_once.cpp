#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    std::once_flag run_once_flag;
    std::once_flag run_once_exceptions_flag;

    auto thread_function = [&] { 
        std::call_once(run_once_flag, []{ 
            std::cout << "This must run just once\n"; 
        }); 
    };

    std::jthread t1(thread_function);
    std::jthread t2(thread_function);
    std::jthread t3(thread_function);

    auto function_throws = [&](bool throw_exception) {
        if (throw_exception) {
            std::cout << "Throwing exception\n";
            throw std::runtime_error("runtime error");
        }

        std::cout << "No exception was thrown\n";
    };

    auto thread_function_1 = [&](bool throw_exception) {
        try {
            std::call_once(run_once_exceptions_flag, function_throws, throw_exception);
        } catch (...) {
        }
    };

    std::jthread t4(thread_function_1, true);
    std::jthread t5(thread_function_1, true);
    std::jthread t6(thread_function_1, false);

    return 0;
}