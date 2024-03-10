#include <thread>
#include <iostream>
#include <string>
#include <random>

namespace {
    int val=0;
    std::mutex mtx;
}

int main() {

    auto work = [&](const std::string& name){
        while(true) {
            bool work_to_do = rand() % 2;
            if (work_to_do) {
                // Do some work: Active wait for 3 second
                std::cout << name << ": working" << std::endl;       

                std::lock_guard<std::mutex> lock(mtx);
                for (auto start = std::chrono::steady_clock::now(), now = start; now < start + std::chrono::seconds{3}; now = std::chrono::steady_clock::now()) {}
            }
            else {
                // Let other threads do some work
                std::cout << name << ": yielding" << std::endl;                
                std::this_thread::yield(); 
            }
        }        
    };

    std::jthread t1(work, "t1");
    std::jthread t2(work, "t2");

    std::cout << "Exit main thread" << std::endl;

    return 0;
}