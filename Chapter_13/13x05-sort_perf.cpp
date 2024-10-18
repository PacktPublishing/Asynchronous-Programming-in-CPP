#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

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

int main() {
    constexpr uint32_t elements = 100000000;
    int32_t minval = 1;
    int32_t maxval = 1000000000;

    auto rv = random_vector(elements, minval, maxval);

    auto start = std::chrono::high_resolution_clock::now();
    std::ranges::sort(rv);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return 0;
}
