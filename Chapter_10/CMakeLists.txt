cmake_minimum_required(VERSION 3.29)
project(BoostCobalt)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.86.0 REQUIRED COMPONENTS thread system container cobalt)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost is required but not found.")
endif()

# Include Boost directories
include_directories(${Boost_INCLUDE_DIRS})

# Create executable for each cpp file
file(GLOB SOURCES "*.cpp")
foreach(SOURCE ${SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE})
    target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES})
endforeach()
