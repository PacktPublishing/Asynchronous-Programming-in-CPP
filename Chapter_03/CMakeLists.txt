# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}/Chapter_03)
message("Binary dir 2: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Create executable for each cpp file
file(GLOB SOURCES "*.cpp")
foreach(SOURCE ${SOURCES})
    message("File: ${SOURCE}")
    get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
    message("File: ${EXECUTABLE_NAME}")
    add_executable(${EXECUTABLE_NAME} ${SOURCE})
    target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES})
endforeach()
