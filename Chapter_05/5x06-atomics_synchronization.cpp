#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

std::string message;
std::atomic<bool> ready{false};

void reader()
{
    while (!ready.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Message received = " << message << std::endl;
}

void writer()
{
    message = "Hello, World!";
    ready.store(true);
}

int main()
{
    std::thread t1(reader);
    std::thread t2(writer);

    t1.join();
    t2.join();

    return 0;
}