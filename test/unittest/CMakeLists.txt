
macro (add_unittest NAME)
    add_executable(${NAME} ${NAME}.cpp)
    target_link_libraries(${NAME} PUBLIC
        GTest::gtest_main
        tones
    )
    add_test(${NAME} ./${NAME})
endmacro(add_unittest)

add_unittest(TestClock)
add_unittest(TestMemory)