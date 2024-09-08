#include <future>
#include <iostream>

int main() {
    std::future<int> empty;
    try {
        int n = empty.get();
        std::cout << "n = " << n << std::endl;
    } catch (const std::future_error& e) {
        std::cerr << "Caught a future_error exception [ " << e.code() << " ] : " << e.what() << std::endl;
    }
}