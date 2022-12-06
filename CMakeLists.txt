cmake_minimum_required(VERSION 3.20)

project(AOC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wuninitialized -Wno-missing-field-initializers -Wno-unused-variable -Wno-restrict")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include lib/cppitertools lib/fmt/include
                    /opt/homebrew/Cellar/boost/1.80.0/include
                    lib/ctre/include)

enable_testing()

# Library Section
add_library(aocutil STATIC src/aoc_util.cpp)
add_subdirectory(lib/fmt)
add_subdirectory(lib/scnlib)
add_subdirectory(lib/ctre)
add_subdirectory(src/2021)
add_subdirectory(src/2022)
