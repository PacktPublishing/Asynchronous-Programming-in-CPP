#include <boost/asio.hpp>
#include <iostream>

int main() {
    boost::asio::io_context io_context;

    io_context.post([] { std::cout << "This will always run asynchronously.\n"; });

    io_context.dispatch([] { std::cout << "This might run immediately or be queued.\n"; });

    io_context.run();
    return 0;
}
