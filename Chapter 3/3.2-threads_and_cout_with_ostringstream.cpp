#include <thread>
#include <iostream>
#include <sstream>

int main() {

    std::thread t1([]() {
        for (int i=0; i<100; ++i) {
            std::ostringstream oss;
            oss << "the quick brown fox jumped over the lazy dog \n";
            std::cout << oss.str();
        }
    });    

    std::thread t2([]() {
        for (int i=0; i<100; ++i) {
            std::ostringstream oss;
            oss << "my mother washes seashells by the sea shore \n";
            std::cout << oss.str();
        }
    });    

    t1.join();
    t2.join();
    
    return 0;
}
