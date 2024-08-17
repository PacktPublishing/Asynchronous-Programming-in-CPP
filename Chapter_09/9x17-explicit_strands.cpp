#include <boost/asio.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

const std::string log_filename = "log.txt";
const unsigned num_threads = 4;
const unsigned num_messages_per_thread = 5;

class Logger {
   public:
    Logger(boost::asio::io_context& io_context, const std::string& filename)
        : strand_(io_context), file_(filename
        , std::ios::out | std::ios::app) 
    {
        if (!file_.is_open()) {
            throw std::runtime_error("Failed to open log file");
        }
    }

    void log(const std::string message) {
        //strand_.post(std::bind(&Logger::do_log, this, message));
        strand_.post([this, message]() { do_log(message); });
    }

   private:
    void do_log(const std::string message) { 
        file_ << message << std::endl; 
    }

    boost::asio::io_context::strand strand_;
    std::ofstream file_;
};

void worker(std::shared_ptr<Logger> logger, int id) {
    for (unsigned i = 0; i < num_messages_per_thread; ++i) {        
        std::ostringstream oss;
        oss << "Thread " << id << " logging message " << i;
        logger->log(oss.str());
        std::this_thread::sleep_for(100ms);
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        auto work_guard = boost::asio::make_work_guard(io_context);
        std::shared_ptr<Logger> logger = std::make_shared<Logger>(io_context, log_filename);

        std::cout << "Logging " << num_messages_per_thread << " messages from " << num_threads << " threads\n";

        std::vector<std::jthread> threads;
        for (unsigned i = 0; i < num_threads; ++i) {
            threads.emplace_back(worker, logger, i);
        }

        threads.emplace_back([&]() { io_context.run_for(2s); });

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}
