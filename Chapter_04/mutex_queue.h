#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>

namespace async_prog {

template <typename T>
class queue {
   public:
    queue(std::size_t capacity) : capacity_{capacity}, buffer_(capacity) {}

    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mtx_);
        not_full_.wait(lock, [this] { return !is_full(); });

        buffer_[tail_] = item;
        tail_ = next(tail_);

        lock.unlock();

        not_empty_.notify_one();
    }

    bool try_push(const T& item) {
        std::unique_lock<std::mutex> lock(mtx_, std::try_to_lock);
        if (!lock || is_full()) {
            return false;
        }

        buffer_[tail_] = item;
        tail_ = next(tail_);

        lock.unlock();

        not_empty_.notify_one();

        return true;
    }

    void pop(T& item) {
        std::unique_lock<std::mutex> lock(mtx_);
        not_empty_.wait(lock, [this] { return !is_empty(); });

        item = buffer_[head_];
        head_ = next(head_);

        lock.unlock();

        not_full_.notify_one();
    }

    bool try_pop(T& item) {
        std::unique_lock<std::mutex> lock(mtx_, std::try_to_lock);
        if (!lock || is_empty()) {
            return false;
        }

        item = buffer_[head_];
        head_ = next(head_);

        lock.unlock();

        not_empty_.notify_one();

        return true;
    }

   private:
    [[nodiscard]] std::size_t next(std::size_t idx) const noexcept { return ((idx + 1) % capacity_); }

    [[nodiscard]] bool is_empty() const noexcept { return (head_ == tail_); }

    [[nodiscard]] bool is_full() const noexcept { return (next(tail_) == head_); }

   private:
    std::mutex mtx_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;

    std::size_t head_{0};
    std::size_t tail_{0};
    std::size_t capacity_;
    std::vector<T> buffer_;
};

}  // namespace async_prog
