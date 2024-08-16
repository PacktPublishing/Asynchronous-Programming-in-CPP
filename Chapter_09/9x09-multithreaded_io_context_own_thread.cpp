#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void completion_handler(const boost::system::error_code& ec) {
    if (!ec) {
        std::cout << "Timer expired successfuly!\n";
    } else {
        std::cout << "Timer error: " << ec.message() << std::endl;
    }
}

void background_task(int i) {
    std::cout << "Thread " << i << ": Starting...\n";
    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);

    std::cout << "Thread " << i << ": Setup timer...\n";
    boost::asio::steady_timer timer(io_context, 1s);
    timer.async_wait(completion_handler);

    std::cout << "Thread " << i << ": Running io_context...\n";
    io_context.run();
}

int main() {
    const int num_threads = 4;
    std::vector<std::jthread> threads;

    for (auto i = 0; i < num_threads; ++i) {
        threads.emplace_back(background_task, i);
    }

    return 0;
}
