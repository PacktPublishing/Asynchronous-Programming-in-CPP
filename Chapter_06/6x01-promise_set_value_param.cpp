#include <future>
#include <iostream>
#include <thread>

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    auto threadFunc = [](std::promise<int> prom) {
        try {
            int result = 42;
            prom.set_value(result);
        } catch (...) {
            prom.set_exception(std::current_exception());
        }
    };

    std::jthread t(threadFunc, std::move(prom));

    try {
        int result = fut.get();
        std::cout << "Result from thread: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
