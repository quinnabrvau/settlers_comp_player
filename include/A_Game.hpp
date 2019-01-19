//
//  A_Game.hpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/12/19.
//

#ifndef A_Game_hpp
#define A_Game_hpp

#include "Game.hpp"
#include "A_Player.hpp"
#include <vector>

class A_Game:public Game {
    std::vector<A_Player> _players;
    std::vector<A_Player*> players;
public:
    A_Game(std::vector<A_Player*> __players, tRollType roll = ROLL_DICE);
    
    
    
};

#endif /* A_Game_hpp */
