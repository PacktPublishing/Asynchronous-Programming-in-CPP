#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void background_task(boost::asio::io_context& io_context) {
    // Waiting for 2 seconds before posting work
    std::this_thread::sleep_for(2s);
    std::cout << "Posting a background task.\n";
    io_context.post([]() { std::cout << "Background task completed!\n"; });
}

int main() {
    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);

    // Work guard avoids run() to return immediately
    std::thread io_thread([&io_context]() {
        std::cout << "Running io_context.\n";
        io_context.run();
        std::cout << "io_context stopped.\n";
    });

    // Creating a thread and posting some work after 2 seconds.
    std::thread worker(background_task, std::ref(io_context));

    // Main thread doing some work.
    std::this_thread::sleep_for(5s);

    // Removing work guard to let the io_context stop.
    std::cout << "Removing work_guard.\n";
    work_guard.reset();

    // Joining threads
    worker.join();
    io_thread.join();
    return 0;
}
