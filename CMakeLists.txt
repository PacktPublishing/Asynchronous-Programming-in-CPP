cmake_minimum_required(VERSION 3.0)
project("Asynchronous Programming in C++")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost
find_package(Boost REQUIRED COMPONENTS thread)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost is required but not found.")
endif()

# Include Boost directories
include_directories(${Boost_INCLUDE_DIRS})

# Linker flags
if((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g3")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

# Set the output directory for binaries
set(BINARY_DIR ${CMAKE_BINARY_DIR}/bin)

# Add chapters
add_subdirectory(Chapter_03)
add_subdirectory(Chapter_04)
add_subdirectory(Chapter_06)
add_subdirectory(Chapter_07)
