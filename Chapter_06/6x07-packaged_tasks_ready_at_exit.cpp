#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <utility>

using namespace std::chrono_literals;

void task_func(std::future<void>& output) {
    std::packaged_task<void(bool&)> task{[](bool& done) { done = true; }};
    auto result = task.get_future();

    bool done = false;
    task.make_ready_at_thread_exit(done);

    std::cout << "task_func: done = " << std::boolalpha << done << std::endl;

    auto status = result.wait_for(0s);
    if (status == std::future_status::timeout)
        std::cout << "task_func: result not ready" << std::endl;

    output = std::move(result);
}

int main() {
    std::future<void> result;

    std::thread t{task_func, std::ref(result)};
    t.join();

    auto status = result.wait_for(0s);
    if (status == std::future_status::ready)
        std::cout << "main: result ready" << std::endl;
    return 0;
}