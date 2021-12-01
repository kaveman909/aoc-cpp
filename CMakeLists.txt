cmake_minimum_required(VERSION 3.20)

project(AOC_2021)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include lib/cppitertools)

enable_testing()

# Library Section
add_library(aocutil STATIC src/aoc_util.cpp)

foreach(x RANGE 1 1)

# AoC Days Section

add_executable(day${x} src/${x}.cpp)
target_link_libraries(day${x} PUBLIC fmt aocutil)

# Test Section

add_test(NAME d${x} COMMAND day${x} $ENV{HOME}/aoc-2021/input/${x}.txt)

endforeach()
