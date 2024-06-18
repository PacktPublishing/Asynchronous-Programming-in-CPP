#include <cmath>
#include <iostream>
#include <vector>
#include <future>
#include <algorithm>
#include <random>

// Generate a large vector of random integers using a uniform distribution
std::vector<int> generate_vector(size_t size) {
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, size);
    std::generate(vec.begin(), vec.end(), [&]() { 
        return dist(gen); 
    });
    return vec;
}

// Search for a value in a segment of a vector
bool search_segment(const std::vector<int>& vec, int target, size_t begin, size_t end) {
    auto begin_it = vec.begin() + begin;
    auto end_it = vec.begin() + end;
    return std::find(begin_it, end_it, target) != end_it;
}

int main() {
    const int target = 100;

    // Large vector
    std::vector<int> vec = generate_vector(5000000);
    auto vec_size = vec.size();

    // Number of segments to divide vector and search
    const size_t num_segments = 16;
    size_t segment_size = vec.size() / num_segments;

    // Launch asynchronous search tasks
    std::vector<std::future<bool>> futs;
    for (size_t i = 0; i < num_segments; ++i) {
        auto begin = std::min(i * segment_size, vec_size);
        auto end = std::min((i + 1) * segment_size, vec_size);
        futs.push_back( std::async(std::launch::async, search_segment, std::cref(vec), target, begin, end) );
    }

    // Collect results
    bool found = false;
    for (auto& fut : futs) {
        if (fut.get()) {
            found = true;
            break;
        }
    }

    if (found) {
        std::cout << "Target " << target << " found in the large vector.\n";
    } else {
        std::cout << "Target " << target << " not found in the large vector.\n";
    }

    return 0;
}
