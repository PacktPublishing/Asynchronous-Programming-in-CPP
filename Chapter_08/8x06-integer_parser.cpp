#include <cassert>
#include <chrono>
#include <coroutine>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <utility>

using namespace std::string_literals;
using namespace std::chrono_literals;

template <typename Out, typename In>
struct async_parse {
    struct promise_type {
        std::optional<In> input_data { };
        Out output_data { };

        async_parse get_return_object() noexcept {
            return async_parse{ *this };
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

        auto await_transform(char) noexcept {
            struct awaiter {
                promise_type& promise;

                [[nodiscard]] bool await_ready() const noexcept {
                    return promise.input_data.has_value();
                }

                [[nodiscard]] char await_resume() const noexcept {
                    assert (promise.input_data.has_value());
                    return *std::exchange(promise.input_data, std::nullopt);
                }

                void await_suspend(std::coroutine_handle<promise_type>) const noexcept {
                }
            };

            return awaiter(*this);

        }
    };

    std::coroutine_handle<promise_type> handle{};

    explicit async_parse(promise_type& promise) : handle{ std::coroutine_handle<promise_type>::from_promise(promise)} {
    }

    ~async_parse() noexcept {
        if (handle) {
            handle.destroy();
        }
    }

    void put(char c) {
        handle.promise().input_data = c;
        if (!handle.done()) {
            handle.resume();
        }
    }

    int64_t get() {
        if (!handle.done()) {
            handle.resume();
        }
        return handle.promise().output_data;
    }
};

async_parse<int64_t, char> parse_string() {
    while (true) {
        char c = co_await char{ };
        int64_t number { };
        int64_t sign { 1 };

        if (c != '-' && c != '+' && !std::isdigit(c)) {
            continue;
        }

        if (c == '-') {
            sign = -1;
        }
        else if (std::isdigit(c)) {
            number = number * 10 + c - '0';
        }

        while (true) {
            c = co_await char{};
            if (std::isdigit(c)) {
                number = number * 10 + c - '0';
            }
            else {
                break;
            }
        }

        co_yield number * sign;
    }
}

int64_t parse_string(const std::string& str) {
    int64_t num{ 0 };
    int64_t sign { 1 };

    std::size_t c = 0;
    while (c < str.size()) {
        if (str[c] == '-') {
            sign = -1;
        }
        else if (std::isdigit(str[c])) {
            num = num * 10ll + (str[c] - '0');
        }
        else if (str[c] == '#') {
            break;
        }
        ++c;
    }

    return num * sign;
}

int main() {
    std::string num1 = "-123454321#"s;
    std::string num2 = "-123454321#98765#-20241337#+31415#"s;

    // parse full string
    std::cout << parse_string(num1) << std::endl;

    // parse the first half of the string
    std::size_t sz = num1.size();
    async_parse<int64_t, char> num_parser = parse_string();
    for (std::size_t i = 0; i < sz / 2; ++i) {
        num_parser.put(num1[i]);
    }

    std::cout << "parsing number...\n"s;

    // parse the second half of the string
    for (std::size_t i = sz / 2; i < sz; ++i) {
        num_parser.put(num1[i]);
    }

    // print the parsed number
    std::cout << num_parser.get() << std::endl;

    // now let's parse a string containing several numbers
    for (char c : num2) {
        num_parser.put(c);

        // do something...

        std::this_thread::sleep_for(1s);

        if (c == '#') {
            std::cout << num_parser.get() << std::endl;
        }
    }

    return 0;
}