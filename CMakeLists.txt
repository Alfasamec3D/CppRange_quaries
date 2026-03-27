cmake_minimum_required(VERSION 3.11)

project("Range_quaries" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(trees STATIC lib/trees.cpp)

target_include_directories(trees PUBLIC "include")

add_executable(main main.cpp)

target_link_libraries (main PRIVATE trees)


option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()