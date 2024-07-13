#include <atomic>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

constexpr int NUM_THREADS{ 8 };

void process()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    int sleep_duration = dis(gen);

    std::this_thread::sleep_for(std::chrono::microseconds(sleep_duration));
}

int main()
{
    std::atomic<int> init_thread{ 0 };

    auto worker = [&init_thread](int i)
    {
        process();

        int init_value = init_thread.load(std::memory_order::seq_cst);
        if (init_value == 0)
        {
            int expected = 0;
            if (init_thread.compare_exchange_strong(expected, i, std::memory_order::seq_cst))
            {
                std::cout << "Previous value of init_thread: " << expected << "\n";
                std::cout << "Thread " << i << " initialized\n";
            }
            else
            {
                // init_thread was already initialized
            }
        }
        else
        {
            // init_thread was already initialized
        }
    };


    std::vector<std::thread> threads;
    for (int i = 1; i <= NUM_THREADS; ++i)
    {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    std::cout << "Thread: " << init_thread.load() << " initialized\n";

    return 0;
}