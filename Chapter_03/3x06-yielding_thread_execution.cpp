#include <iostream>
#include <random>
#include <string>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

namespace {
int val = 0;
std::mutex mtx;
}

int main() {
    auto work = [&](const std::string& name) {
        while (true) {
            bool work_to_do = rand() % 2;
            if (work_to_do) {
                // Do some work: Active wait for 3 second
                sync_cout << name << ": working" << std::endl;
                std::lock_guard<std::mutex> lock(mtx);
                auto start = std::chrono::steady_clock::now();
                for (auto now = start;
                          now < start + 3s;
                          now = std::chrono::steady_clock::now()) {
                }
            } else {
                // Let other threads do some work
                sync_cout << name << ": yielding" << std::endl;
                std::this_thread::yield();
            }
        }
    };

    // These threads will work forever as there is no exit condition in the work lambda
    std::jthread t1(work, "t1");
    std::jthread t2(work, "t2");

    return 0;
}