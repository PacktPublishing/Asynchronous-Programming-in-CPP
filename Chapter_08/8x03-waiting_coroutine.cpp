#include <coroutine>
#include <iostream>
#include <string>

using namespace std::string_literals;

struct return_type {
    struct promise_type {
        std::string input_data { };

        return_type get_return_object() noexcept {
            std::cout << "get_return_object" << std::endl;
            return return_type{ *this };
        }

        void return_void() noexcept {
            std::cout << "return_void" << std::endl;
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

        auto await_transform(std::string) noexcept {
            struct awaiter {
                promise_type& promise;

                bool await_ready() const noexcept {
                    std::cout << "await_ready()" << std::endl;
                    return true;
                }

                std::string await_resume() const noexcept {
                    std::cout << "await_resume()" << std::endl;
                    return std::move(promise.input_data);
                }

                void await_suspend(std::coroutine_handle<promise_type>) const noexcept {
                    std::cout << "await_suspend" << std::endl;
                }
            };

            return awaiter(*this);

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

    void put(std::string msg) {
        handle.promise().input_data = std::move(msg);
        if (!handle.done()) {
            handle.resume();
        }
    }
};

return_type coro_func() {
    std::cout << co_await std::string{ };
    co_return;
}

int main() {
    auto rt = coro_func();
    rt.put("Hello from main\n"s);
    return 0;
}