#include <cmath>
#include <functional>
#include <future>
#include <iostream>
#include <thread>

void task_thread() {
    std::packaged_task<int(int, int)> task(std::pow<int, int>);
    std::future<int> result = task.get_future();
    std::jthread t(std::move(task), 2, 10);
    std::cout << "Task thread: " << result.get() << '\n';
}

void task_lambda() {
    std::packaged_task<int(int, int)> task([](int a, int b) { return std::pow(a, b); });
    std::future<int> result = task.get_future();
    task(2, 10);
    std::cout << "Task lambda: " << result.get() << '\n';
}

void task_bind() {
    std::packaged_task<int()> task(std::bind(std::pow<int, int>, 2, 10));
    std::future<int> result = task.get_future();
    task();
    std::cout << "Task bind: " << result.get() << '\n';
}

int main() {
    task_thread();
    task_lambda();
    task_bind();
}