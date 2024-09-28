#include <chrono>
#include <iostream>

#include <boost/cobalt.hpp>

using namespace std::chrono_literals;

boost::cobalt::generator<int, int> fibonacci_term() {
    auto fibonacci = [](int n) {
        if (n < 2) {
            return n;
        }

        int f0 = 0;
        int f1 = 1;
        int f;

        for (int i = 2; i < n; ++i) {
            f = f0 + f1;
            f0 = f1;
            f1 = f;
        }

        return f;
    };

    auto x = co_await boost::cobalt::this_coro::initial;
    while (x != -1) {
        x = co_yield fibonacci(x);
    }

    co_return 0;
}

boost::cobalt::generator<int> fibonacci_sequence() {
    int f0 = 0;
    int f1 = 1;
    int f = 0;

    while (true) {
        co_yield f0;

        f = f0 + f1;
        f0 = f1;
        f1 = f;
    }
}

boost::cobalt::main co_main(int, char*[]) {
    auto g = fibonacci_sequence();

    // print first 10 fibonacci numbers
    for (int i = 0; i < 10; ++i) {
        std::cout << co_await g << std::endl;
    }

    // do something...
    std::this_thread::sleep_for(2s);

    // print 10 more fibonacci numbers
    for (int i = 0; i < 10; ++i) {
        std::cout << co_await g << std::endl;
    }

    // print first 10 even fibonacci numbers
    auto h = fibonacci_term();
    for (int i = 0; i < 20; i += 2) {
        std::cout << i << "  " << co_await h(i) << std::endl;
    }

    co_return 0;
}