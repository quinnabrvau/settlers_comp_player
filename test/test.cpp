
#include "Game.hpp"
#include "unity.h"

int main(int argc, const char * argv[]) {
    Game game0;
    Game game1(3,10);
    game1.play_game();
    return 0;
}