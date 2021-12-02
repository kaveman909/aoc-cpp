cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER $ENV{HOME}/GCC/bin/gcc)
set(CMAKE_CXX_COMPILER $ENV{HOME}/GCC/bin/g++)

project(AOC_2021)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include lib/cppitertools lib/fmt/include)

enable_testing()

# Library Section
add_library(aocutil STATIC src/aoc_util.cpp)
add_subdirectory(lib/fmt)

# Determine how many Advent days to build
execute_process(COMMAND ls src/days
                COMMAND wc -l
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                OUTPUT_VARIABLE NUM_FILES)

foreach(x RANGE 1 ${NUM_FILES})

# AoC Days Section

add_executable(day${x} src/days/${x}.cpp)
target_link_libraries(day${x} PUBLIC fmt aocutil)

# Test Section

add_test(NAME d${x} COMMAND day${x} $ENV{HOME}/aoc-2021/input/${x}.txt)

endforeach()
