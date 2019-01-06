project( TEST )

cmake_minimum_required (VERSION 3.4)

file(GLOB_RECURSE TEST_C_SOURCES   "*.c")
file(GLOB_RECURSE TEST_CPP_SOURCES "*.cpp")
file(GLOB_RECURSE UNITY_SOURCES "../src/*.c")

include_directories (
    ../include
    ../unity/src
)
add_definitions (
    -DTESTING
)
add_executable (Test 
    ${TEST_C_SOURCES}
    ${TEST_CPP_SOURCES}
    ${UNITY_SOURCES}
)
target_link_libraries(Test
    GAME_LIB
)

add_test (NAME MyTest COMMAND Test)