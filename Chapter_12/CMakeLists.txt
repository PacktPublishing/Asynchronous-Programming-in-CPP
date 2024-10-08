# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}/Chapter_12)

# Option for using Clang complier
# Enabled in this subfolder by default
option(USE_CLANG "Build examples with Clang" ON)
if(USE_CLANG)
    message("Using Clang in Chapter 12")

    set(CMAKE_CXX_COMPILER             "/usr/bin/clang++")
    set(CMAKE_CXX_FLAGS                "-Wall")
    set(CMAKE_CXX_FLAGS_DEBUG          "-g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

    set(CMAKE_AR                       "/usr/bin/llvm-ar")
    set(CMAKE_LINKER                   "/usr/bin/llvm-ld")
    set(CMAKE_NM                       "/usr/bin/llvm-nm")
    set(CMAKE_OBJDUMP                  "/usr/bin/llvm-objdump")
    set(CMAKE_RANLIB                   "/usr/bin/llvm-ranlib")
else()
    message("Using default compiler in Chapter 12")
endif()


# Create executable for each cpp file
file(GLOB SOURCES "*.cpp")
foreach(SOURCE ${SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE})
    target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES})

    # Add compiler and linker flags depending on type of sanitizer
    if (EXECUTABLE_NAME MATCHES ".*ASAN.*")
        message("Using AddressSanitizer for ${EXECUTABLE_NAME}")
        target_compile_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer -g -O0)
        target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address)
    elseif (EXECUTABLE_NAME MATCHES ".*LSAN.*")
        message("Using LeakSanitizer for ${EXECUTABLE_NAME}")
        target_compile_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=leak -fno-omit-frame-pointer -g -O2)
        target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=leak)
    elseif (EXECUTABLE_NAME MATCHES ".*MSAN.*")
        message("Using MemorySanitizer for ${EXECUTABLE_NAME}")
        # -fPIE -pie
        target_compile_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=memory -fPIE -fno-omit-frame-pointer -g -O2)
        target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=memory)
    elseif (EXECUTABLE_NAME MATCHES ".*TSAN.*")
        message("Using ThreadSanitizer for ${EXECUTABLE_NAME}")
        target_compile_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=thread -fno-omit-frame-pointer -g -O2)
        target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=thread)
    elseif (EXECUTABLE_NAME MATCHES ".*UBSAN.*")
        message("Using UndefinedBehaviorSanitizer for ${EXECUTABLE_NAME}")
        target_compile_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=undefined -fno-omit-frame-pointer -g -O2)
        target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=undefined)
    elseif (EXECUTABLE_NAME MATCHES ".*test.*")
        message("Enable testing for ${EXECUTABLE_NAME}")
        target_link_libraries(${EXECUTABLE_NAME} GTest::GTest GTest::Main GTest::gmock_main)
        gtest_discover_tests(${EXECUTABLE_NAME})
    else()
        message("Sanitizer not enabled for ${EXECUTABLE_NAME}")
    endif()

endforeach()
