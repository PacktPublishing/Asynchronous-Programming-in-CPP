/** To compile this example: 
 * g++ -std=c++20 Chapter\ 3/3.5-check_hardware_concurrency.cpp -L/usr/local/lib/ -lboost_thread
*/

#include <thread>
#include <iostream>
#include <boost/thread.hpp>

int main() {
    const auto processor_count = std::thread::hardware_concurrency();
    std::cout << "Processors: " << processor_count << std::endl;

    const auto nthreads = boost::thread::hardware_concurrency();
    std::cout << "Threads: " << nthreads << std::endl;

    return 0;    
}