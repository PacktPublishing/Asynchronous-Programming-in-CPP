#include <atomic>
#include <cassert>
#include <iostream>
#include <vector>
#include <thread>

#include <pthread.h>

void set_affinity(int core) {
    if (core < 0) {
        return;
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
        exit(EXIT_FAILURE);
    }
}

template<typename T>
class spsc_lock_free_queue {
public:
    // capacity must be power of two to avoid using modulo operator when calculating the index
    explicit spsc_lock_free_queue(size_t capacity) : capacity_(capacity), buffer_(capacity) {
        assert((capacity & (capacity - 1)) == 0 && "capacity must be a power of 2");
    }

    spsc_lock_free_queue(const spsc_lock_free_queue &) = delete;

    spsc_lock_free_queue &operator=(const spsc_lock_free_queue &) = delete;

    bool empty() const {
        return tail_.load(std::memory_order_relaxed) == head_.load(std::memory_order_relaxed);
    }

    bool push(const T &item) {
        std::size_t tail = tail_.load(std::memory_order_relaxed);
        std::size_t next_tail = (tail + 1) & (capacity_ - 1);
        if (next_tail != head_.load(std::memory_order_acquire)) {
            buffer_[tail] = item;
            tail_.store(next_tail, std::memory_order_release);
            return true;
        }

        return false;
    }

    bool pop(T &item) {
        std::size_t head = head_.load(std::memory_order_relaxed);
        if (head == tail_.load(std::memory_order_acquire)) {
            return false;
        }

        item = buffer_[head];
        head_.store((head + 1) & (capacity_ - 1), std::memory_order_release);

        return true;
    }

private:
    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::atomic<std::size_t> tail_{0};
    const std::size_t capacity_;
    std::vector<T> buffer_;
};

template<typename T>
class spsc_lock_free_queue_optimized {
public:
    // capacity must be power of two to avoid using modulo operator when calculating the index
    explicit spsc_lock_free_queue_optimized(size_t capacity) : capacity_(capacity), buffer_(capacity) {
        assert((capacity & (capacity - 1)) == 0 && "capacity must be a power of 2");
    }

    spsc_lock_free_queue_optimized(const spsc_lock_free_queue_optimized &) = delete;

    spsc_lock_free_queue_optimized &operator=(const spsc_lock_free_queue_optimized &) = delete;

    bool empty() const {
        return tail_.load(std::memory_order_relaxed) == head_.load(std::memory_order_relaxed);
    }

    bool push(const T &item) {
        std::size_t tail = tail_.load(std::memory_order_relaxed);
        std::size_t next_tail = (tail + 1) & (capacity_ - 1);
        if (next_tail == cache_head_) {
            cache_head_ = head_.load(std::memory_order_acquire);
            if (next_tail == cache_head_) {
                return false;
            }
        }

        buffer_[tail] = item;
        tail_.store(next_tail, std::memory_order_release);
        return true;
    }

    bool pop(T &item) {
        std::size_t head = head_.load(std::memory_order_relaxed);
        if (head == cache_tail_) {
            cache_tail_ = tail_.load(std::memory_order_acquire);
            if (head == cache_tail_) {
                return false;
            }
        }

        item = buffer_[head];
        head_.store((head + 1) & (capacity_ - 1), std::memory_order_release);
        return true;
    }

private:
    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::atomic<std::size_t> tail_{0};
    alignas(64) std::size_t cache_head_{0};
    alignas(64) std::size_t cache_tail_{0};
    const std::size_t capacity_;
    std::vector<T> buffer_;
};

template <typename T>
void consumer(T &queue, int core, int iterations) {
    set_affinity(core);
    for (int i = 0; i < iterations; ++i) {
        int data;
        while (!queue.pop(data)) {
        }
        if (data != i) {
            throw std::runtime_error("");
        }
    }
}

void test_spsc_lock_free_queue(int64_t iterations, int producer_core, int consumer_core) {
    spsc_lock_free_queue<int> queue(1 << 17);

    std::thread th(consumer<spsc_lock_free_queue<int>>, std::ref(queue), consumer_core, iterations);
    set_affinity(producer_core);

    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        while (!queue.push(i)) {
        }
    }
    while (!queue.empty()) {
    }
    th.join();
    const auto end = std::chrono::steady_clock::now();

    std::cout << iterations * 1000000000 / std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
              << " ops/sec\n";
}

void test_optimized_spsc_lock_free_queue(int64_t iterations, int producer_core, int consumer_core) {
    spsc_lock_free_queue_optimized<int> queue(1 << 17);

    std::thread th(consumer<spsc_lock_free_queue_optimized<int>>, std::ref(queue), consumer_core, iterations);
    set_affinity(producer_core);

    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        while (!queue.push(i)) {
        }
    }
    while (!queue.empty()) {
    }
    th.join();
    const auto end = std::chrono::steady_clock::now();

    std::cout << iterations * 1000000000 / std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
              << " ops/sec\n";
}

int main() {
    const int producer_core = 2;
    const int consumer_core = 12;
    const long long iterations = 1000000000LL;

    test_spsc_lock_free_queue(iterations, producer_core, consumer_core);
    test_optimized_spsc_lock_free_queue(iterations, producer_core, consumer_core);

    return 0;
}
