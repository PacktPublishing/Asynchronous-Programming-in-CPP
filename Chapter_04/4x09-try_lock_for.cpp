#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

constexpr int NUM_THREADS = 8;
int counter = 0;
int failed = 0;

int main()
{
    using namespace std::chrono_literals;

    std::timed_mutex mutex;
    std::mutex m;

    auto worker = [&] {
        for (int i = 0; i < 10; ++i) {
            if (mutex.try_lock_for(10ms)) {
                ++counter;
                std::cout << "Counter: " << counter << std::endl;
                std::this_thread::sleep_for(10ms);
                mutex.unlock();
            }
            else {
                m.lock();
                ++failed;
                std::cout << "Thread " << std::this_thread::get_id() << " failed to lock" << std::endl;
                m.unlock();
            }
			
            std::this_thread::sleep_for(12ms);
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(worker);
    }

    for (auto& t : threads) {
        t.join();
    }
	
    std::cout << "Counter: " << counter << std::endl;
    std::cout << "Failed: " << failed << std::endl;

    return 0;
}
