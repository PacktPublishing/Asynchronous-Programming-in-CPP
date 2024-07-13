#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std::literals;

int main() {
    std::promise<std::string> prom;
    std::future<std::string> fut = prom.get_future();

    auto t1 = std::jthread([prm = std::move(prom)] mutable {
        std::this_thread::sleep_for(100ms);
        prm.set_value("Value successfully set.");
        // We could also use: prm.set_value_at_thread_exit("Value successfully set."s);
    });

    std::cout << fut.get() << std::endl;

    std::promise<int> other_prom;
    std::future<int> other_fut = other_prom.get_future();
    auto t2 = std::jthread([prm = std::move(other_prom)] mutable {
        try {
            throw std::runtime_error("Throwing internal exception.");
            prm.set_value(1);
        } catch (...) {
            prm.set_exception(std::current_exception());
            // We could also use: prm.set_exception_at_thread_exit(std::current_exception());
        }
    });

    try {
        auto val = other_fut.get();
        std::cout << "This will not be printed: " << val << '\n';
    } catch (const std::exception& e) {
        std::cout << "Propagated exception: " << e.what() << '\n';
    }

    return 0;
}