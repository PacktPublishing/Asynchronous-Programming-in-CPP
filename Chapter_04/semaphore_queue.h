#pragma once

#include <mutex>
#include <semaphore>
#include <vector>

namespace async_prog {

template <typename T>
class semaphore_queue {
   public:
    semaphore_queue(std::size_t capacity)
        : sem_empty_(capacity), sem_full_(0), capacity_{capacity}, buffer_(capacity) {}

    void push(const T& item) {
        sem_empty_.acquire();

        std::unique_lock<std::mutex> lock(mtx_);

        buffer_[tail_] = item;
        tail_ = next(tail_);

        lock.unlock();

        sem_full_.release();
    }

    bool try_push(const T& item) {
        if (!sem_empty_.try_acquire()) {
            return false;
        }

        std::unique_lock<std::mutex> lock(mtx_);

        buffer_[tail_] = item;
        tail_ = next(tail_);

        lock.unlock();

        sem_full_.release();

        return true;
    }

    void pop(T& item) {
        sem_full_.acquire();

        std::unique_lock<std::mutex> lock(mtx_);

        item = buffer_[head_];
        head_ = next(head_);

        lock.unlock();

        sem_empty_.release();
    }

    bool try_pop(T& item) {
        if (!sem_full_.try_acquire()) {
            return false;
        }

        std::unique_lock<std::mutex> lock(mtx_);

        item = buffer_[head_];
        head_ = next(head_);

        lock.unlock();

        sem_empty_.release();

        return true;
    }

   private:
    [[nodiscard]] std::size_t next(std::size_t idx) const noexcept { return ((idx + 1) % capacity_); }

   private:
    std::mutex mtx_;
    std::counting_semaphore<> sem_empty_;
    std::counting_semaphore<> sem_full_;

    std::size_t head_{0};
    std::size_t tail_{0};
    std::size_t capacity_;
    std::vector<T> buffer_;
};

}  // namespace async_prog
