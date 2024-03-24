#include <iostream>
#include <thread>

thread_local int val = 0;

void setValue(int newval) { val = newval; }

void printValue() { std::cout << val; }

void multiplyByTwo(int arg) {
    // The thread_local value is set and multiplied by 2
    setValue(arg);
    val *= 2;
    printValue();
}

int main() {
    val = 1;  // Value in main thread

    // Each thread set its own value
    std::thread t1(multiplyByTwo, 1);
    std::thread t2(multiplyByTwo, 2);
    std::thread t3(multiplyByTwo, 3);

    t1.join();
    t2.join();
    t3.join();

    std::cout << val << std::endl;

    return 0;
}