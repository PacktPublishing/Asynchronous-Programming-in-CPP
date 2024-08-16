#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>

using boost::asio::ip::tcp;

constexpr int port = 1234;

void handle_client(tcp::socket& socket) {
    const size_t size_buffer = 5;
    boost::asio::streambuf buf1, buf2;

    std::array<boost::asio::mutable_buffer, 2> buffers = {
        buf1.prepare(size_buffer),
        buf2.prepare(size_buffer)
    };

    boost::system::error_code ec;
    size_t bytes_recv = socket.read_some(buffers, ec);
    if (ec) {
        std::cerr << "Error on receive: " << ec.message() << '\n';
        return;
    }
    std::cout << "Received " << bytes_recv << " bytes\n";

    // Extract data from buffers
    buf1.commit(5);
    buf2.commit(5);

    std::istream is1(&buf1);
    std::istream is2(&buf2);

    std::string data1, data2;
    is1 >> data1;
    is2 >> data2;

    std::cout << "Buffer 1: " << data1 << std::endl;
    std::cout << "Buffer 2: " << data2 << std::endl;
}

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
        std::cout << "Server is running on port " << port << "...\n";

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Client connected...\n";

            handle_client(socket);
            std::cout << "Client disconnected...\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}
