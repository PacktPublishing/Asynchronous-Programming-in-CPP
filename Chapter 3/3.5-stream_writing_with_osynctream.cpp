#include <iostream>
#include <syncstream>
#include <thread>

int main() {
    std::osyncstream sync_out(std::cout);

    std::thread t1([&]() {
        for (int i = 0; i < 100; ++i) {
            sync_out << "1 " << "2 " << "3 " << "4 " << std::endl;
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 100; ++i) {
            sync_out << "5 " << "6 " << "7 " << "8 " << std::endl;
        }
    });

    t1.join();
    t2.join();

    return 0;
}
