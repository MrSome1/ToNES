
find_package(GTest REQUIRED)

set(TEST_ROOT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(ROM_ROOT_DIR ${TEST_ROOT_DIR}/roms)

add_subdirectory(roms)
add_subdirectory(unittest)

if (${BUILD_DEBUGGER})
    add_subdirectory(simulator)
endif()
