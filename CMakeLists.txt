cmake_minimum_required(VERSION 3.11)

project("Range_quaries" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest REQUIRED)

add_executable(main main.cpp)

add_executable(tests tests/tests.cpp)

add_library(trees src/trees.cpp)

include_directories("include")

target_link_libraries (main trees)

target_link_libraries(tests trees gtest gtest_main)

add_test(NAME TreeEndTest COMMAND tests)