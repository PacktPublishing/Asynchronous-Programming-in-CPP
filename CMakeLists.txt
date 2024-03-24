cmake_minimum_required(VERSION 3.0)
project("Asynchronous Programming in C++")

# Set the output directory for binaries
message("Binary output: ${CMAKE_BINARY_DIR}")
set(BINARY_DIR, ${CMAKE_BINARY_DIR}/bin)

# Add chapters
add_subdirectory(Chapter_03)
