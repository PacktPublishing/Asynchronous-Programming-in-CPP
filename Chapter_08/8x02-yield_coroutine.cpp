#include <coroutine>
#include <iostream>
#include <string>

using namespace std::string_literals;

struct return_type {
    struct promise_type {
        std::string output_data { };

        return_type get_return_object() noexcept {
            std::cout << "get_return_object" << std::endl;
            return return_type{ *this };
        }

        void return_void() noexcept {
            std::cout << "return_void" << std::endl;
        }

        std::suspend_always yield_value(std::string msg) noexcept {
            std::cout << "yield_value" << std::endl;
            output_data = std::move(msg);
            return {};
        }

        std::suspend_always initial_suspend() noexcept {
            std::cout << "initial_suspend" << std::endl;
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            std::cout << "final_suspend" << std::endl;
            return {};
        }

        void unhandled_exception() noexcept {
            std::cout << "unhandled_exception" << std::endl;
        }
    };

    std::coroutine_handle<promise_type> handle{};

    explicit return_type(promise_type& promise) : handle{ std::coroutine_handle<promise_type>::from_promise(promise)} {
        std::cout << "return_type()" << std::endl;
    }

    ~return_type() noexcept {
        if (handle) {
            handle.destroy();
        }

        std::cout << "~return_type()" << std::endl;
    }

    std::string get() {
        std::cout << "get()" << std::endl;

        if (!handle.done()) {
            handle.resume();
        }
        return std::move(handle.promise().output_data);
    }
};

return_type coro_func() {
    co_yield "Hello from the coroutine\n"s;

    co_return;
}

int main() {
    auto rt = coro_func();
    std::cout << rt.get() << std::endl;
    return 0;
}