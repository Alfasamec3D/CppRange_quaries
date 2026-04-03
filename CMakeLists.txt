cmake_minimum_required(VERSION 3.11)

project("Range_quaries" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)

target_include_directories (main PRIVATE "include")


option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()