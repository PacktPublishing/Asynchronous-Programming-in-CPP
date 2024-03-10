#include <thread>
#include <iostream>

int main() {

    std::thread t1([]() {
        for (int i=0; i<100; ++i) {
            std::cout << "the quick " << "brown fox " << "jumped over " << "the lazy dog " << std::endl;
        }
    });

    std::thread t2([]() {
        for (int i=0; i<100; ++i) {
            std::cout << "my mother " << "washes " << "seashells " << "by the sea shore" << std::endl;
        }
    });

    t1.join();
    t2.join();
    
    return 0;
}
