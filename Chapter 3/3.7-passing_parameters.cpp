#include <iostream>
#include <string>
#include <thread>
#include <vector>

// Passing arguments by value
void funcByValue(const std::string& str, int val) { std::cout << "str: " << str << ", val: " << val << std::endl; }

// Passing arguments by reference
void modifyValues(std::string& str, int& val) {
    str += " (Thread)";
    val++;
}

void printVector(const std::vector<int>& v) {
    std::cout << "Vector: ";
    for (int num : v) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Passing arguments by value
    std::string str1{"Passing by value"};
    std::thread t1(funcByValue, str1, 1);
    t1.join();

    // Passing arguments by reference
    std::string str2{"Passing by reference"};
    int val = 1;
    std::thread t2(modifyValues, std::ref(str2), std::ref(val));
    t2.join();
    std::cout << "str: " << str2 << ", val: " << val << std::endl;

    // Passing argument by const reference
    std::vector<int> v{1, 2, 3, 4, 5};
    std::thread t3a(printVector, std::cref(v));
    t3a.join();

    // Moving element into a thread
    std::thread t3b(printVector, std::move(v));
    t3b.join();

    // Note: Trying to access v here would result in undefined behavior
    // as V was moved into the thread and not usable anymore.

    // Lambda function with captures
    std::string str4{"Hello"};
    std::thread t4([&]() { std::cout << "str: " << str4 << ", val: " << val << std::endl; });
    t4.join();

    return 0;
}
