#include <chrono>
#include <iostream>

#include <boost/cobalt.hpp>

using namespace std::chrono_literals;

boost::cobalt::generator<int> basic_generator()
{
    std::this_thread::sleep_for(1s);
    co_yield 1;
    std::this_thread::sleep_for(1s);
    co_return 0;
}

boost::cobalt::main co_main(int, char*[]) {
    auto g = basic_generator();

    std::cout << co_await g << std::endl;
    std::cout << co_await g << std::endl;

    co_return 0;
}