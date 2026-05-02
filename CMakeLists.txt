cmake_minimum_required(VERSION 3.18)

project(matmul_optimization
        VERSION 0.1
        DESCRIPTION "Matrix multiplication optimization"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(matmul
  src/main.cpp
  src/matrix.cpp
  src/matmul_naive.cpp
  src/benchmark.cpp
  src/matmul_tiled_cpu.cpp)
target_include_directories(matmul PRIVATE include)
target_compile_options(matmul PRIVATE -march=native -Wall -Wextra)
