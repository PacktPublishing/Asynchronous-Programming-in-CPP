#include <boost/asio.hpp>
#include <iostream>
#include <sstream>
#include <string>

using boost::asio::ip::tcp;

boost::asio::awaitable<void> echo(tcp::socket socket) {
    char data[1024];

    while (true) {
        std::cout << "Reading data from socket...\n";
        std::size_t bytes_read = co_await socket.async_read_some(boost::asio::buffer(data), boost::asio::use_awaitable);

        if (bytes_read == 0) {
            std::cout << "No data. Exitting loop...\n";
            break;
        }

        // Remove /r/n from end of received data
        std::string str(data, bytes_read);
        if (!str.empty() && str.back() == '\n') {
            str.pop_back();
                }
        if (!str.empty() && str.back() == '\r') {
            str.pop_back();
        }

        // If the client send QUIT, the server closes the connection
        if (str == "QUIT") {
            std::string bye("Good bye!\n");
            co_await boost::asio::async_write(socket, boost::asio::buffer(bye), boost::asio::use_awaitable);
            break;
        }

        std::cout << "Writing '" << str << "' back into the socket...\n";
        co_await boost::asio::async_write(socket, boost::asio::buffer(data, bytes_read), boost::asio::use_awaitable);
    }
}

boost::asio::awaitable<void> listener(boost::asio::io_context& io_context, unsigned short port) {
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

    while (true) {
        std::cout << "Accepting connections...\n";
        tcp::socket socket = co_await acceptor.async_accept(boost::asio::use_awaitable);

        std::cout << "Starting an Echo connection handler...\n";
        boost::asio::co_spawn(io_context, echo(std::move(socket)), boost::asio::detached);
    }
}

int main() {
    boost::asio::io_context io_context;

    try {
        boost::asio::co_spawn(io_context, listener(io_context, 12345), boost::asio::detached);
        io_context.run();

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        io_context.stop();
    }

    return 0;
}
