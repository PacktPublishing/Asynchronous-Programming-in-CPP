#include <boost/asio.hpp>
#include <iostream>
#include <stdexcept>

void my_handler() {
    try {
        std::cout << "Starting work in handler...\n";
        throw std::runtime_error("An error occurred during the operation");
        std::cout << "Handler completed work successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }
}

void post_handler(boost::asio::io_context& io_context) { 
    io_context.post(my_handler); 
}

int main() {
    boost::asio::io_context io_context;
    post_handler(io_context);
    io_context.run();
    return 0;
}
