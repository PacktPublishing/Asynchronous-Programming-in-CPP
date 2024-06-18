#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <syncstream>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

int square(const std::string& task_name, int x) {
    sync_cout << "Launching " << task_name << " task...\n";
    return x * x;
}

int main() {
    sync_cout << "Starting main thread...\n";

    auto fut_async = std::async(std::launch::async, 
                            square, "async_policy", 2);
    auto fut_deferred = std::async(std::launch::deferred, 
                            square, "deferred_policy", 3);
    auto fut_default = std::async(square, 
                            "default_policy", 4);

    sync_cout << "Waiting in main thread...\n";
    std::this_thread::sleep_for(1s);
    sync_cout << "Wait in main thread finished.\n";

    sync_cout << "Getting result from "
              << "async policy task...\n";
    int val_async = fut_async.get();
    sync_cout << "Result from async policy task: " 
              << val_async << '\n';

    sync_cout << "Getting result from "
              << "deferred policy task...\n";
    int val_deferred = fut_deferred.get();
    sync_cout << "Result from deferred policy task: " 
              << val_deferred << '\n';

    sync_cout << "Getting result from "
              << "default policy task...\n";
    int val_default = fut_default.get();
    sync_cout << "Result from default policy task: " 
              << val_default << '\n';

    return 0;
}
