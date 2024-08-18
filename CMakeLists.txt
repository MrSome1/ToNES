
cmake_minimum_required(VERSION 3.10)

project(ToNES VERSION 0.1)

set(CMAKE_BUILD_TYPE Debug)
message("Build Type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTING "Compile the test cases" OFF)

INCLUDE_DIRECTORIES(./include)

file(GLOB HEADEDS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(tones SHARED
    ${HEADERS}
    ${SOURCES}
)

include(CTest)

if (${BUILD_TESTING})
    enable_testing()
    add_subdirectory(test)
endif()
