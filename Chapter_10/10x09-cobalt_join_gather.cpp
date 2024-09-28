#include <iostream>
#include <random>
#include <variant>

#include <boost/asio.hpp>
#include <boost/cobalt.hpp>

using namespace std::chrono_literals;

boost::cobalt::promise<std::chrono::milliseconds::rep> process(std::chrono::milliseconds ms) {
    if (ms > std::chrono::milliseconds(5000)) {
        throw std::runtime_error("delay throw");
    }

    boost::asio::steady_timer tmr{ co_await boost::cobalt::this_coro::executor, ms };
    co_await tmr.async_wait(boost::cobalt::use_op);
    co_return ms.count();
}

boost::cobalt::main co_main(int, char*[]) {
    auto result = co_await boost::cobalt::join(process(100ms),
                                               process(200ms),
                                               process(300ms));

    std::cout << "First coroutine finished in: "
              <<  std::get<0>(result) << "ms\n";
    std::cout << "Second coroutine took finished in: "
              <<  std::get<1>(result) << "ms\n";
    std::cout << "Third coroutine took finished in: "
             <<  std::get<2>(result) << "ms\n";

    try {
        auto result_throw = co_await boost::cobalt::join(process(100ms),
                                                         process(20000ms),
                                                         process(300ms));
    }
    catch (...) {
        std::cout << "An exception was thrown\n";
    }

    try {
        auto result_throw = co_await boost::cobalt::gather(process(100ms),
                                                           process(20000ms),
                                                           process(300ms));


        if (std::get<0>(result_throw).has_value()) {
            std::cout << "First coroutine took: "
                      <<  *std::get<0>(result_throw) << "msec\n";
        }
        else {
            std::cout << "First coroutine threw an exception\n";
        }
        if (std::get<1>(result_throw).has_value()) {
            std::cout << "Second coroutine took: "
                      <<  *std::get<1>(result_throw) << "msec\n";
        }
        else {
            std::cout << "Second coroutine threw an exception\n";
        }
        if (std::get<2>(result_throw).has_value()) {
            std::cout << "Third coroutine took: "
                      <<  *std::get<2>(result_throw) << "msec\n";
        }
        else {
            std::cout << "Third coroutine threw an exception\n";
        }
    }
    catch (...) {
        // this is never reached because gather doesn't throw exceptions
        std::cout << "An exception was thrown\n";
    }

    co_return 0;
}