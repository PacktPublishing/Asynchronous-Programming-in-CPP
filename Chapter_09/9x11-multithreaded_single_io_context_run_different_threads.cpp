#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

int main() {
    boost::asio::io_context io_context;

    boost::asio::steady_timer timer(io_context, 2s);
    timer.async_wait([](const boost::system::error_code& /*ec*/) {
        std::cout << "Timer expired!\n";
    });

    const std::size_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&io_context]() {
            io_context.run();
        });
    }

    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
