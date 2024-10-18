# Include Benchmark library
find_package(benchmark REQUIRED)

# Set compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS "-O3 -DNDEBUG -std=c++20 -march=native")

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}/Chapter_13)

# Create executable for each cpp file
file(GLOB SOURCES "*.cpp")
foreach(SOURCE ${SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE})
endforeach()