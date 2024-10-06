#!/bin/bash

# Install GNU GCC 12, 11, and 10
add-apt-repository ppa:ubuntu-toolchain-r/ppa -y
apt update
apt install -y g++-12 gcc-12 g++-11 gcc-11 g++-10 gcc-10 build-essential

# Install GNU GCC 13
add-apt-repository ppa:ubuntu-toolchain-r/test -y
apt install -y gcc-13 g++-13

# Install GNU GCC14
apt install -y gcc-14 g++-14

# Setup alternatives: Use `update-alternatives --config gcc` to select a specifc compiler.
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 60 --slave /usr/bin/g++ g++ /usr/bin/g++-14 --slave /usr/bin/gcov gcov /usr/bin/gcov-14
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 80 --slave /usr/bin/g++ g++ /usr/bin/g++-13 --slave /usr/bin/gcov gcov /usr/bin/gcov-13
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 --slave /usr/bin/g++ g++ /usr/bin/g++-12 --slave /usr/bin/gcov gcov /usr/bin/gcov-12
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 40 --slave /usr/bin/g++ g++ /usr/bin/g++-11 --slave /usr/bin/gcov gcov /usr/bin/gcov-11
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 20 --slave /usr/bin/g++ g++ /usr/bin/g++-10 --slave /usr/bin/gcov gcov /usr/bin/gcov-10

# Install CLang and LLVM tools
apt install -y clang clang-format clang-tidy clang-tools llvm

# Install CMake
apt install -y cmake

# Install GoogleTest
apt install -y libgtest-dev
