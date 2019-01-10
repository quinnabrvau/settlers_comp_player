project( GAME )
cmake_minimum_required (VERSION 3.4)

file(GLOB_RECURSE headers "../include/*.hpp")

set( GAME_SOURCES
    ./Board.cpp
    ./Game.cpp
    ./Player.cpp
    ./common.cpp
    ${headers}
)
set( GENETIC_SOURCES
    ./Genetic.cpp
)

include_directories(
    ../include
)

add_library(GAME_TEST_LIB
    ../unity/src/unity.c
    ${GAME_SOURCES}
)
target_include_directories(GAME_TEST_LIB
    PRIVATE ../unity/src
)
target_compile_definitions(GAME_TEST_LIB
    PRIVATE TESTING
)

add_library(GENETIC_TEST_LIB
    ../unity/src/unity.c
    ${GENETIC_SOURCES}
)
target_include_directories(GENETIC_TEST_LIB
    PRIVATE ../unity/src
)
target_compile_definitions(GENETIC_TEST_LIB
    PRIVATE TESTING
)

add_executable(catan
    ${GAME_SOURCES}
    ${GENETIC_SOURCES}
    ./main.cpp
)
