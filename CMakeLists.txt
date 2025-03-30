
cmake_minimum_required(VERSION 3.10)

project(ToNES VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTING "Compile the test cases" OFF)
option(BUILD_DEBUGGER "Compile the debugging simulator" OFF)

INCLUDE_DIRECTORIES(./include)

file(GLOB HEADEDS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(tones SHARED
    ${HEADERS}
    ${SOURCES}
)

if (${BUILD_TESTING})
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()
