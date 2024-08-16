#include <boost/asio.hpp>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context, 1s);

    auto fut = timer.async_wait(boost::asio::use_future);

    std::thread io_thread([&io_context]() { io_context.run(); });

    std::this_thread::sleep_for(3s);

    // Cancel timer, throwing an exception that is caught by the future.
    timer.cancel();

    try {
        fut.get();
        std::cout << "Timer expired successfully!\n";
    } catch (const boost::system::system_error& e) {
        std::cout << "Timer failed: " << e.code().message() << '\n';
    }

    io_thread.join();
    return 0;
}
