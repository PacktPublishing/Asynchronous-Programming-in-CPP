#include <gtest/gtest.h>
#include <coroutine>
#include <exception>
#include <iostream>

struct Task {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        int result_;
        std::exception_ptr exception_;

        Task get_return_object() {
            return Task(handle_type::from_promise(*this));
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        void return_value(int value) { 
            result_ = value; 
        }

        void unhandled_exception() {
            exception_ = std::current_exception();
        }
    };

    handle_type handle_;

    Task(handle_type h) : handle_(h) {}

    ~Task() {
        if (handle_) handle_.destroy();
    }

    bool await_ready() const noexcept { 
        return handle_.done(); 
    }

    void await_suspend(std::coroutine_handle<> awaiting_handle) {
        handle_.resume();
        awaiting_handle.resume();
    }

    int await_resume() {
        if (handle_.promise().exception_) {
            std::rethrow_exception(handle_.promise().exception_);
        }
        return handle_.promise().result_;  // Access the result from the promise
    }

    int result() {
        if (handle_.promise().exception_) {
            std::rethrow_exception(handle_.promise().exception_);
        }
        return handle_.promise().result_;
    }
};

// A coroutine that doubles the input value
Task asyncFunc(int x) {
    co_return 2 * x;
}

// A coroutine that throws an exception
Task asyncFuncWithException() {
    throw std::runtime_error("Exception from coroutine");
    co_return 0;
}

Task TestCoroutineHelper(int value) {
    co_return co_await asyncFunc(value);
}

Task TestCoroutineWithExceptionHelper() {
    co_return co_await asyncFuncWithException();
}

TEST(AsyncTest, TestCoroutine) {
    auto task = TestCoroutineHelper(5);
    task.handle_.resume();
    EXPECT_EQ(task.result(), 10);
}

TEST(AsyncTest, TestCoroutineWithException) {
    auto task = TestCoroutineWithExceptionHelper();
    EXPECT_THROW({
            task.handle_.resume();
            task.result();
        },
        std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
