
#include "Game.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "common.hpp"
#include "unity.h"

int main(int argc, const char * argv[]) {
    UNITY_BEGIN();

    test__board_edge();
    test__board_node();
    
    test__player();
    
    return UNITY_END();
}
