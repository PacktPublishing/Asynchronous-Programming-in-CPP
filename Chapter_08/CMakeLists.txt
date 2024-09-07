cmake_minimum_required(VERSION 3.29)
project(coroutines)

set(CMAKE_CXX_STANDARD 23)

add_executable(coroutines 8x01-simple_coroutine.cpp
        8x02-yield_coroutine.cpp
        8x03-waiting_coroutine.cpp
        8x06-integer_parser.cpp
        8x04-fibonacci_generator.cpp
        8x05-generator_cpp23.cpp)
