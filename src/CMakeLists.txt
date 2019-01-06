project( GAME )
cmake_minimum_required (VERSION 3.4)

set( GAME_SOURCES
    ./Board.cpp
    ./Game.cpp
    ./Player.cpp
)

add_library(GAME_LIB
    ${GAME_SOURCES}
)
add_executable(catan
    ${GAME_SOURCES}
    ./main.cpp
)