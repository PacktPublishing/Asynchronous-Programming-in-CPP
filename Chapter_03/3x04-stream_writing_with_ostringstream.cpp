#include <iostream>
#include <sstream>
#include <thread>

int main() {
    std::thread t1([]() {
        for (int i = 0; i < 100; ++i) {
            std::ostringstream oss;
            oss << "1 " << "2 " << "3 " << "4 " << "\n";
            std::cout << oss.str();
        }
    });

    std::thread t2([]() {
        for (int i = 0; i < 100; ++i) {
            std::ostringstream oss;
            oss << "5 " << "6 " << "7 " << "8 " << "\n";
            std::cout << oss.str();
        }
    });

    t1.join();
    t2.join();

    return 0;
}
