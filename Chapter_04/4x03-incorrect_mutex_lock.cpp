#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    std::deque<int> dq;

    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);

    dq.pop_front();

    std::cout << dq[0] << std::endl;

    std::mutex mtx;

    auto func = [&] {
        // double lock
        mtx.lock();
        mtx.lock();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        // double unlock
        mtx.unlock();
        mtx.unlock();
        
        // mutex destroyed with the thread still running and owning the lock
    };

    std::thread t1(func);
    // t1.join();
    return 0;
}