#include <boost/asio.hpp>
#include <chrono>
#include <functional>
#include <iostream>

using namespace std::chrono_literals;

int main() {
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context, 3s);

    std::function<void(const boost::system::error_code&)> timer_handler;
    timer_handler = [&timer, &timer_handler](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "Handler: Timer expired." << std::endl;
            timer.expires_after(1s);
            timer.async_wait(timer_handler);
        } else {
            std::cerr << "Handler error: " << ec.message() << std::endl;
        }
    };
    timer.async_wait(timer_handler);
    io_context.run();
    return 0;
}
