#include <future>
#include <iostream>
#include <stdexcept>

int stage1(int x) {
    if (x < 0) throw std::runtime_error("Negative input not allowed");
    return x + 10;
}

int stage2(int x) {
    return x * 2;
}

int stage3(int x) {
    return x - 5;
}

int main() {
    int input_value = 5;

    try {
        auto fut1 = std::async(std::launch::async, stage1, input_value);

        auto fut2 = std::async(std::launch::async, [&fut1]() { return stage2(fut1.get()); });

        auto fut3 = std::async(std::launch::async, [&fut2]() { return stage3(fut2.get()); });

        int final_result = fut3.get();
        std::cout << "Final Result: " << final_result << std::endl;

    } catch (const std::exception &ex) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
    }

    return 0;
}
