project( GAME )
cmake_minimum_required (VERSION 3.4)

file(GLOB_RECURSE headers "../include/*.hpp")

set( GAME_SOURCES
    ./Board.cpp
    ./Game.cpp
    ./Player.cpp
    ./common.cpp
    ../unity/src/unity.c
    ${headers}
)

include_directories(
    ../include
    ../unity/src
)

add_library(GAME_LIB
    ${GAME_SOURCES}
)
add_executable(catan
    ./main.cpp
)
target_link_libraries(catan
    GAME_LIB
)
