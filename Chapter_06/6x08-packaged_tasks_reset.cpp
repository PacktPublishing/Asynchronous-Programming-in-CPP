#include <cmath>
#include <future>
#include <iostream>
#include <thread>

int main() {
    // Reusing a packaged task by resetting it
    std::packaged_task<int(int, int)> task([](int a, int b) { return std::pow(a, b); });

    for (int i=1; i<=10; ++i) {
        std::future<int> result = task.get_future();
        task(2, i);
        std::cout << "2^" << i << " = " << result.get() << std::endl;
        task.reset();
    }

    return 0;
}