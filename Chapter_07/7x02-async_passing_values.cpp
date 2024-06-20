#include <iostream>
#include <string>
#include <syncstream>
#include <future>
#include <vector>

#define sync_cout std::osyncstream(std::cout)

// Passing arguments by value
void funcByValue(const std::string& str, int val) {
    sync_cout << "str: " << str << ", val: " << val << std::endl;
}

// Passing arguments by reference
void modifyValues(std::string& str, int& val) {
    str += " (Thread)";
    val++;
}

void printVector(const std::vector<int>& v) {
    sync_cout << "Vector: ";
    for (int num : v) {
        sync_cout << num << " ";
    }
    sync_cout << std::endl;
}

int main() {
    // Passing arguments by value
    std::string str1{"Passing by value"};
    auto fut1 = async(funcByValue, str1, 1);

    // Passing arguments by reference
    std::string str2{"Passing by reference"};
    int val = 1;
    auto fut2 = std::async(modifyValues, std::ref(str2), std::ref(val));
    fut2.get();
    sync_cout << "str: " << str2 << ", val: " << val << std::endl;

    // Passing argument by const reference
    std::vector<int> v{1, 2, 3, 4, 5};
    auto fut3 = std::async(printVector, std::cref(v));
    fut3.get();

    // Moving element into a thread
    auto fut4 = std::async(printVector, std::move(v));
    fut4.get();

    // Note: Trying to access v here would result in undefined behavior
    // as V was moved into the thread and not usable anymore.

    // Lambda function with captures
    std::string str5{"Hello"};
    auto fut5 = std::async([&]() {
        sync_cout << "str: " << str5 << std::endl;
    });
    fut5.get();

    return 0;
}
