
#include <fmt/core.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

int main() {
    uint32_t counter1{};
    std::mutex mtx1;

    uint32_t counter2{};
    std::mutex mtx2;

    // Create a multi-sink logger logging into console and a file simultaneously
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logging.log", true);
    file_sink->set_level(spdlog::level::info);

    spdlog::logger logger("multi_sink", {console_sink, file_sink});
    logger.set_pattern("%Y-%m-%d %H:%M:%S.%f - Thread %t [%l] : %v");
    logger.set_level(spdlog::level::debug);

    auto increase_and_swap = [&]() {
        logger.info("Incrementing both counters...");
        counter1++;
        counter2++;

        logger.info("Swapping counters...");
        std::swap(counter1, counter2);
    };

    auto worker1 = [&]() {
        logger.debug("Entering worker1");

        logger.info("Locking mtx1...");
        std::lock_guard<std::mutex> lock1(mtx1);
        logger.info("Mutex mtx1 locked");

        std::this_thread::sleep_for(100ms);

        logger.info("Locking mtx2...");
        std::lock_guard<std::mutex> lock2(mtx2);
        logger.info("Mutex mtx2 locked");

        increase_and_swap();

        logger.debug("Leaving worker1");
    };

    auto worker2 = [&]() {
        logger.debug("Entering worker2");

        logger.info("Locking mtx2...");
        std::lock_guard<std::mutex> lock2(mtx2);
        logger.info("Mutex mtx2 locked");

        std::this_thread::sleep_for(100ms);

        logger.info("Locking mtx1...");
        std::lock_guard<std::mutex> lock1(mtx1);
        logger.info("Mutex mtx1 locked");

        increase_and_swap();

        logger.debug("Leaving worker2");
    };

    logger.debug("Starting main function...");

    std::thread t1(worker1);
    std::thread t2(worker2);

    t1.join();
    t2.join();

    spdlog::dump_backtrace();
    return 0;
}
