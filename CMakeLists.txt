cmake_minimum_required(VERSION 3.18)

project(matmul_optimization
        VERSION 0.1
        DESCRIPTION "Matrix multiplication optimization"
        LANGUAGES CXX)

enable_testing()

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_executable(matmul
  src/main.cpp
  src/matrix.cpp
  src/matmul_naive.cpp
  src/benchmark.cpp
  src/matmul_tiled_cpu.cpp
  src/matmul_tiled_threaded.cpp)
target_include_directories(matmul PRIVATE include)
target_compile_options(matmul PRIVATE -march=native -Wall -Wextra)
target_link_libraries(matmul PRIVATE Threads::Threads)

add_executable(test_naive
  tests/test_naive.cpp
  src/matrix.cpp
  src/matmul_naive.cpp)
target_include_directories(test_naive PRIVATE 
  include
  tests)
target_compile_options(test_naive PRIVATE -march=native -Wall -Wextra)
add_test(NAME test_naive COMMAND test_naive)
