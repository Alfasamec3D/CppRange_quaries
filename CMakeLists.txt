cmake_minimum_required(VERSION 3.11)

project("Range_quaries" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest REQUIRED)

add_library(trees STATIC src/trees.cpp)

target_include_directories(trees PUBLIC "include")

add_executable(main main.cpp)

target_link_libraries (main PRIVATE trees)

add_executable(tests tests/tests.cpp)

target_link_libraries(tests PRIVATE trees gtest gtest_main)

add_test(NAME TreeEndTest COMMAND tests)