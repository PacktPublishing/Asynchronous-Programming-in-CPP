#include <future>
#include <iostream>
#include <thread>
#include <tuple>

using namespace std::literals;

// combineFunc will run in a thread, spanning 2 extra threads,
// one for computing a value, another to retrieve a value from a file.
// Both results will be returned to the main thread as a combined promise.
void combineFunc(std::promise<std::tuple<int, std::string>> combineProm) {
    try {
        // Thread to simulate computing a value.
        std::cout << "Starting computeThread..." << std::endl;
        auto computeVal = [](std::promise<int> prom) mutable {
            std::this_thread::sleep_for(1s);
            prom.set_value(42);
        };
        std::promise<int> computeProm;
        auto computeFut = computeProm.get_future();
        std::jthread computeThread(computeVal, std::move(computeProm));

        // Thread to simulate downloading a file.
        std::cout << "Starting dataThread..." << std::endl;
        auto fetchData = [](std::promise<std::string> prom) mutable {
            std::this_thread::sleep_for(2s);
            prom.set_value("data.txt"s);
        };
        std::promise<std::string> fetchProm;
        auto fetchFut = fetchProm.get_future();
        std::jthread dataThread(fetchData, std::move(fetchProm));

        combineProm.set_value({computeFut.get(), fetchFut.get()});
    } catch (...) {
        combineProm.set_exception(std::current_exception());
    }
}

int main() {
    // Create combined promise. Get results, waiting for threads
    // if not finished yet.
    std::cout << "Creating combined promise..." << std::endl;
    std::promise<std::tuple<int, std::string>> combineProm;
    auto combineFuture = combineProm.get_future();
    std::jthread combineThread(combineFunc, std::move(combineProm));

    // Access results in the main thread
    auto [data, file] = combineFuture.get();
    std::cout << "Value [ " << data << " ]  File [ " << file << " ]" << std::endl;

    return 0;
}
