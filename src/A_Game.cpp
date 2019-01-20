//
//  A_Game.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/12/19.
//

#include "A_Game.hpp"

std::vector<float> A_Game_Wrapper(
        std::vector<A_Player*> players, void* context) {
    tRollType roll = ROLL_DICE;
    if (context != NULL) {
        roll = * (tRollType*)context;
    }
    A_Game game(players, roll);
    game.play_game();
    return game.eval_game();
}
