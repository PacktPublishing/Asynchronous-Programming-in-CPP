#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

const unsigned VALUE_RANGE = 1000; 
const unsigned RESULTS_TO_PRODUCE = 10;  // Numbers of items to produce.
const unsigned MAX_WAIT = 500;           // Maximum waiting time (ms) when producing items.


template <typename T>
class ThreadSafeQueue {
   public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
        cond_var_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [&] { return !queue_.empty(); });
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

   private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};

using TaskQueue = ThreadSafeQueue<std::future<int>>;

void producer(TaskQueue& queue, int val) {
    // Create a new promise
    std::promise<int> prom;
    auto fut = prom.get_future();

    // Move future into the queue
    queue.push(std::move(fut));

    // Simulate some work in the producer side before setting the value in the promise
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % MAX_WAIT));

    // Set the value in the promise
    prom.set_value(val);
}

void consumer(TaskQueue& queue) {
    // Simulate some work in the consumer side before receiving the future
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % MAX_WAIT));

    // Extract a future from the queue
    std::future<int> fut = queue.pop();

    // Retrieve the result from the future
    try {
        int result = fut.get();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    TaskQueue queue;

    auto producerFunc = [](TaskQueue& queue) {
        auto n = RESULTS_TO_PRODUCE;
        while (n-- > 0) {
            int val = rand() % VALUE_RANGE;
            std::cout << "Producer: Sending value " << val << std::endl;
            producer(queue, val);
        }
    };

    auto consumerFunc = [](TaskQueue& queue) {
        auto n = RESULTS_TO_PRODUCE;
        while (n-- > 0) {
            std::cout << "Consumer: Receiving value" << std::endl;
            consumer(queue);
        }
    };

    std::jthread producerThread(producerFunc, std::ref(queue));
    std::jthread consumerThread(consumerFunc, std::ref(queue));

    return 0;
}
