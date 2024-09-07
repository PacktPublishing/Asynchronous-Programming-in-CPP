#include <coroutine>
#include <iostream>

struct return_type {
    struct promise_type {
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
    };

    explicit return_type(promise_type& promise) {
        std::cout << "return_type()" << std::endl;
    }

    ~return_type() noexcept {
        std::cout << "~return_type()" << std::endl;
    }
};

return_type coro_func() {
    co_return;
}

int main() {
    auto rt = coro_func();
    return 0;
}