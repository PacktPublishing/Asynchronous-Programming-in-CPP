#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

void handle_timer_expiry(boost::asio::steady_timer& timer, int count) {
    std::cout << "Timer expired. Count: " << count << std::endl;
    timer.expires_after(1s);
    timer.async_wait([&timer, count](const boost::system::error_code& ec) {
        if (!ec) {
            handle_timer_expiry(timer, count + 1);
        } else {
            std::cerr << "Error: " << ec.message() << std::endl;
        }
    });
}

int main() {
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context, 1s);
    int count = 0;
    timer.async_wait([&](const boost::system::error_code& ec) {
        if (!ec) {
            handle_timer_expiry(timer, count);
        } else {
            std::cerr << "Error: " << ec.message() << std::endl;
        }
    });
    io_context.run();
    return 0;
}
