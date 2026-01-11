cmake_minimum_required(VERSION 3.14)
project(HighPerformanceWrappers VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directory
include_directories(include)

# Core Library
add_library(cpp_core SHARED
    src/TimeSeriesProcessor.cpp
)

target_include_directories(cpp_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(cpp_core PRIVATE CPP_CORE_EXPORTS)

if(MSVC)
    target_compile_options(cpp_core PRIVATE /W4 /WX)
else()
    target_compile_options(cpp_core PRIVATE -Wall -Wextra -Werror -O3)
endif()

# Fetch Google Benchmark
include(FetchContent)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# Benchmarks
add_executable(core_benchmarks benchmarks/main_benchmark.cpp)
target_link_libraries(core_benchmarks PRIVATE cpp_core benchmark::benchmark)
