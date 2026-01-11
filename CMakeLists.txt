cmake_minimum_required(VERSION 3.14)
project(HighPerformanceWrappers_Root)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(cpp_core)
add_subdirectory(csharp_wrapper/NativeLib)
