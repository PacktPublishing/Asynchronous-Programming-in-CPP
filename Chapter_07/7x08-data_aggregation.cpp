#include <future>
#include <iomanip>
#include <iostream>
#include <vector>

int square(int x) { return x * x; }

int sum_results(std::vector<std::future<int>>& futures) {
    int sum = 0;
    for (auto& fut : futures) {
        sum += fut.get();
    }
    return sum;
}

double average_squares(int n) {
    std::vector<std::future<int>> futures;

    for (int i = 1; i <= n; ++i) {
        futures.push_back(std::async(std::launch::async, square, i));
    }

    return double(sum_results(futures)) / n;
}

int main() {
    int N = 100;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average of squares for N = " << N << " is " << average_squares(N) << '\n';
    return 0;
}
