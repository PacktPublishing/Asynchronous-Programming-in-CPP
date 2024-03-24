#include <iostream>
#include <string>
#include <syncstream>
#include <thread>
#include <vector>

namespace {
std::osyncstream sync_out(std::cout);
}

// Passing arguments by value
void funcByValue(const std::string& str, int val) { 
    sync_out << "str: " << str << ", val: " << val << std::endl; 
}

// Passing arguments by reference
void modifyValues(std::string& str, int& val) {
    str += " (Thread)";
    val++;
}

void printVector(const std::vector<int>& v) {
    sync_out << "Vector: ";
    for (int num : v) {
        sync_out << num << " ";
    }
    sync_out << std::endl;
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
    sync_out << "str: " << str2 << ", val: " << val << std::endl;

    // Passing argument by const reference
    std::vector<int> v{1, 2, 3, 4, 5};
    std::thread t3(printVector, std::cref(v));
    t3.join();

    // Moving element into a thread
    std::thread t4(printVector, std::move(v));
    t4.join();

    // Note: Trying to access v here would result in undefined behavior
    // as V was moved into the thread and not usable anymore.

    // Lambda function with captures
    std::string str5{"Hello"};
    std::thread t5([&]() { sync_out << "str: " << str5 << ", val: " << val << std::endl; });
    t5.join();

    return 0;
}
