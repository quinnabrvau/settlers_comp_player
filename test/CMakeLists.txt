project( TEST )

cmake_minimum_required (VERSION 3.4)


file(GLOB_RECURSE UNITY_SOURCES "../unity/src/*")

include_directories (
    ../include
    ../matrix/include
    ../genetic/include
    ../unity/src
)
add_executable (_Game_Test 
    game_test.cpp
    ${UNITY_SOURCES}
)
add_executable (_Play_Test 
    play_test.cpp
    ${UNITY_SOURCES}
)
add_executable (_A_Game_Test
    a_game_test.cpp
    ${UNITY_SOURCES}
)
target_link_libraries(_Game_Test
    GAME_TEST_LIB
)
target_link_libraries(_Play_Test
    GAME_TEST_LIB
)
target_link_libraries(_A_Game_Test
    GAME_TEST_LIB
    ADV_GAME_TEST_LIB
)

add_test (NAME Game_Test COMMAND _Game_Test)
add_test (NAME Play_Test COMMAND _Play_Test)
add_test (NAME A_Game_Test COMMAND _A_Game_Test)
