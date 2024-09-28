#include <iostream>

#include <boost/cobalt.hpp>

boost::cobalt::generator<int, int> square_generator(int x)
{
    while (x != 0) {
        x = co_yield x * x;
    }

    co_return 0;
}

boost::cobalt::main co_main(int, char*[]) {
    auto g = square_generator(10);

    std::cout << co_await g(4) << std::endl;
    std::cout << co_await g(12) << std::endl;
    std::cout << co_await g(0) << std::endl;

    co_return 0;
}