#include <chrono>
#include <iostream>
#include <thread>

#include <boost/cobalt.hpp>

void sleep() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

boost::cobalt::promise<int> eager_promise() {
    std::cout << "Eager promise started\n";
    sleep();
    std::cout << "Eager promise done\n";
    co_return 1;
}

boost::cobalt::task<int> lazy_task() {
    std::cout << "Lazy task started\n";
    sleep();
    std::cout << "Lazy task done\n";
    co_return 2;
}

boost::cobalt::main co_main(int, char*[]) {
    std::cout << "Calling eager_promise...\n";
    auto promise_result = eager_promise();
    std::cout << "Promise called, but not yet awaited.\n";

    std::cout << "Calling lazy_task...\n";
    auto task_result = lazy_task();
    std::cout << "Task called, but not yet awaited.\n";

    std::cout << "Awaiting both results...\n";
    int promise_value = co_await promise_result;
    std::cout << "Promise value: " << promise_value
              << std::endl;

    int task_value = co_await task_result;
    std::cout << "Task value: " << task_value
              << std::endl;

    co_return 0;
}