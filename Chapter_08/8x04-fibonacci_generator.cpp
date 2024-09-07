#include <cassert>
#include <coroutine>
#include <iostream>


using namespace std::string_literals;

template <typename Out>
struct sequence_generator {
    struct promise_type {
        Out output_data { };

        sequence_generator get_return_object() noexcept {
            return sequence_generator{ *this };
        }

        void return_void() noexcept {
        }

        std::suspend_always initial_suspend() noexcept {
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            return {};
        }

        void unhandled_exception() noexcept {
        }

        std::suspend_always yield_value(int64_t num) noexcept {
            output_data = num;
            return {};
        }
    };

    std::coroutine_handle<promise_type> handle{};

    explicit sequence_generator(promise_type& promise) : handle{ std::coroutine_handle<promise_type>::from_promise(promise)} {
    }

    ~sequence_generator() noexcept {
        if (handle) {
            handle.destroy();
        }
    }

    void next() {
        if (!handle.done()) {
            handle.resume();
        }
    }

    int64_t value() {
        return handle.promise().output_data;
    }
};

sequence_generator<int64_t> fibonacci() {
    int64_t a{ 0 };
    int64_t b{ 1 };
    int64_t c{ 0 };

    while (true) {
        co_yield a;
        c = a + b;
        a = b;
        b = c;
    }
}

int main() {
    sequence_generator<int64_t> fib = fibonacci();

    std::cout << "Generate ten Fibonacci numbers\n"s;

    for (int i = 0; i < 10; ++i) {
        fib.next();
        std::cout << fib.value() << " ";
    }
    std::cout << std::endl;

    std::cout << "Generate ten more\n"s;

    for (int i = 0; i < 10; ++i) {
        fib.next();
        std::cout << fib.value() << " ";
    }
    std::cout << std::endl;

    std::cout << "Let's do five more\n"s;

    for (int i = 0; i < 5; ++i) {
        fib.next();
        std::cout << fib.value() << " ";
    }
    std::cout << std::endl;

    return 0;
}