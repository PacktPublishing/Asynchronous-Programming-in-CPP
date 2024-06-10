#include <boost/thread.hpp>
#include <iostream>
#include <thread>

int main() {
    const auto processor_count = std::thread::hardware_concurrency();
    std::cout << "Processors: " << processor_count << std::endl;

    const auto nthreads = boost::thread::hardware_concurrency();
    std::cout << "Processors: " << nthreads << std::endl;

    return 0;
}