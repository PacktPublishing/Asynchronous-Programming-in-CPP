#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class spin_lock
{
public:
    spin_lock() = default;
    spin_lock(const spin_lock&) = delete;
    spin_lock& operator=(const spin_lock&) = delete;

    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
        {
        }
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

constexpr uint32_t NUM_THREADS{ 4 };
uint32_t counter{ 0 };
std::atomic<uint32_t> atomic_counter{ 0 };

int main()
{
    std::mutex m;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([&m]
            {
                for (int i = 0; i < 200000000; ++i)
                {
                    m.lock();
                    counter++;
                    m.unlock();
                }
            });
    }
    for (auto& t : threads)
    {
        t.join();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = stop - start;
    std::cout << "Elapsed time: " << elapsed.count() << std::endl;
    std::cout << counter << std::endl;

    spin_lock sl;
    counter = 0;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i] = std::thread([&sl]
            {
                for (int i = 0; i < 500000000; ++i)
                {
                    sl.lock();
                    counter++;
                    sl.unlock();
                }
            });
    }

    for (auto& t : threads)
    {
        t.join();
    }
    stop = std::chrono::high_resolution_clock::now();
    elapsed = stop - start;
    std::cout << "Elapsed time: " << elapsed.count() << std::endl;
    std::cout << counter << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads[i] = std::thread([]()
            {
                for (int i = 0; i < 500000000; ++i)
                {
                    atomic_counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
    }
    for (auto& t : threads)
    {
        t.join();
    }
    stop = std::chrono::high_resolution_clock::now();
    elapsed = stop - start;
    std::cout << "Elapsed time: " << elapsed.count() << std::endl;
    std::cout << atomic_counter << std::endl;

    return 0;
}