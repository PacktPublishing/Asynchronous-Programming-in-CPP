#include <future>
#include <iostream>

int main() {
    std::future<int> empty;
    try {
        int n = empty.get();
    } catch (const std::future_error& e) {
        std::cout << "Caught a future_error exception [ " << e.code() << " ] : " << e.what() << std::endl;
    }
}