#include <iostream>

#include <boost/cobalt.hpp>

boost::cobalt::main co_main(int, char*[]) {
    std::cout << "Hello Boost.Cobalt\n";
    co_return 0;
}