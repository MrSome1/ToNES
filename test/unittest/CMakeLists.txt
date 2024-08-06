
macro (add_unittest NAME)
    set(TEST_NAME ${NAME}_test)
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} PUBLIC
        GTest::gtest_main
        tones
    )
    add_test(${TEST_NAME} ./${TEST_NAME})
endmacro(add_unittest)

add_unittest(Clock)
add_unittest(Device)
add_unittest(MicroProcessor)
