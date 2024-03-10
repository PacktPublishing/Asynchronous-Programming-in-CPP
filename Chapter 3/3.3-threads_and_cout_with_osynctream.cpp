#include <thread>
#include <iostream>
#include <syncstream>

int main() {

    std::thread t1([]() {
        std::osyncstream synced_out(std::cout);
        for (int i=0; i<100; ++i) {
            synced_out << "the quick " << "brown fox " << "jumped over " << "the lazy dog " << std::endl;
        }
    });    

    std::thread t2([]() {
        std::osyncstream synced_out(std::cout);
        for (int i=0; i<100; ++i) {
            synced_out << "my mother " << "washes " << "seashells " << "by the sea shore" << std::endl;
        }
    });    

    t1.join();
    t2.join();
    
    return 0;
}
