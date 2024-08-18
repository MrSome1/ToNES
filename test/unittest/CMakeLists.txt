
include_directories(${ROM_ROOT_DIR})

macro(add_unittest NAME)
    set(TEST_NAME ${NAME}_test)

    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)

    target_link_libraries(${TEST_NAME} PUBLIC
        GTest::gtest_main
        tones)

    add_test(NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endmacro(add_unittest)

add_unittest(Clock)
add_unittest(Device)
add_unittest(Cartridge)
add_unittest(MicroProcessor)
