#include <generator>
#include <iostream>

std::generator<int> fibonacci_generator() {
    int a{ };
    int b{ 1 };

    while (true) {
        co_yield a;
        int c = a + b;
        a = b;
        b = c;
    }
}

std::generator<int> fibonacci_generator(int limit) {
    int a{ };
    int b{ 1 };

    while (limit--) {
        co_yield a;
        int c = a + b;
        a = b;
        b = c;
    }
}

int main() {

    auto fib = fibonacci_generator();


    int i = 0;
    for (auto f = fib.begin(); f != fib.end(); ++f) {
        if (i == 10) {
            break;
        }
        std::cout << *f << " ";
    ++i;
    }
    std::cout << std::endl;

    for (int f : fibonacci_generator(10)) {
        std::cout << f << " ";
    }

    return 0;
}