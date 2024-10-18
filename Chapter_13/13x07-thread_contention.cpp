#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

int uniform_random_number(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

std::vector<int> random_vector(std::size_t n, int32_t min_val, int32_t max_val) {
    std::vector<int> rv(n);
    std::generate(rv.begin(), rv.end(), [&] { return uniform_random_number(min_val, max_val); });
    return rv;
}
struct thread_data {
    std::mutex mtx;
    uint32_t idx { 0 };
    std::vector<int> vec;
    int64_t sum{ 0ll };

    thread_data(uint32_t size, int32_t min_val, int32_t max_val)  : vec{ random_vector(size, min_val, max_val) } {
        std::cout << vec.size() << " " << idx << " " << sum << std::endl;
    }
};

using namespace std::chrono;

int main() {
    const uint32_t size = 1000000000;
    const int min_val = 1;
    const int max_val = 10;
    thread_data data(size, min_val, max_val);

    auto start = high_resolution_clock::now();
    std::thread th([&] {
        for (uint32_t i = 0; i < data.vec.size(); i++) {
            data.sum += data.vec[i];
        }
    });
    th.join();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    std::cout << "Single thread: " << duration.count() << "usec sum = " << data.sum << std::endl;

    data.idx = 0;
    data.sum = 0ll;
    int num_threads = 8;
    std::vector<std::thread> threads;
    start = high_resolution_clock::now();
    for (uint32_t i = 0; i < num_threads; i++) {
        threads.emplace_back([&] {
            while (true) {
                data.mtx.lock();
                if (data.idx == data.vec.size()) {
                    data.mtx.unlock();
                    return;
                }
                data.sum += data.vec[data.idx++];
                data.mtx.unlock();
            }
        });
    }
    for (auto& t : threads) {
        t.join();
    }
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    std::cout << "Thread contention: " << duration.count() << "usec sum " << data.sum << std::endl;

    return 0;
}