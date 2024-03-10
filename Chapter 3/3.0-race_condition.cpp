#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

struct Worker {
    int& acc;

    Worker(int& acc) : acc(acc) {}

    void operator()(int x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        acc = acc + x * x;
    }
};

int main() {
    int result = 0;

    std::vector<std::thread> thread_list{};
    for (int i = 1; i <= 10; i++) {
        thread_list.push_back(std::thread{Worker(result), i});
    }

    for (auto& t : thread_list) {
        t.join();
    }
    std::cout << " result   = " << result << std::endl;
    return 0;
}