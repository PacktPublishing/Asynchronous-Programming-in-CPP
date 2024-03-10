#include <thread>
#include <iostream>

thread_local int val = 0;

void setValue(int newval) {
    val = newval;
}

void printValue() {
    std::cout << val;
}

void thread_func(int arg) {
    // The thread_local value is set and multiplied by 2
    setValue(arg);
    val *= 2;
    printValue();
}

int main() {
    val = 1;  // Value in main thread

    // Each thread set its own value
    std::thread t1(thread_func, 1);
    std::thread t2(thread_func, 2);
    std::thread t3(thread_func, 3);

    t1.join();
    t2.join();
    t3.join();

    std::cout << val << std::endl;

    return 0;    
}