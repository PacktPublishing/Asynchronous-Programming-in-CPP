#include <format>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
uint32_t counter{};

void function_throws() { throw std::runtime_error("Error"); }

int main() {
    auto worker = [] {
        for (int i = 0; i < 1000000; ++i) {
            mtx.lock();
            counter++;
            mtx.unlock();
        }
    };

    auto worker_try = [] {
        int i = 0;
        while (i < 1000000) {
            if (mtx.try_lock()) {
                ++counter;
                ++i;
                mtx.unlock();
            }
        }
    };

    auto worker_exceptions = [] {
        for (int i = 0; i < 1000000; ++i) {
            try {
                std::lock_guard<std::mutex> lock(mtx);
                counter++;
                function_throws();
            } catch (std::system_error& e) {
                std::cout << e.what() << std::endl;
                return;
            } catch (...) {
                return;
            }
        }
    };

    std::thread t1(worker_exceptions);
    std::thread t2(worker);

    t1.join();
    t2.join();

    std::cout << std::format("Final counter value: {0}\n", counter);

    //std::lock_guard<std::mutex> lock1();
}