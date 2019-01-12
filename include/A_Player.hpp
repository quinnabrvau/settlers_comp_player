//
//  A_Player.hpp
//  GENETIC_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef A_Player_hpp
#define A_Player_hpp

#include "Array.hpp"
#include "Player.hpp"
#include <vector>

typedef Array<float> State;

class A_Player:public Player {
    std::vector<int> dims;
    std::vector<Array<float>> array;
    
    Array<float> calculate(State state);
    
public:
    A_Player(std::vector<Array<float>> _array, int p = 0);
    
    // take a turn
    // given a set of moves (think start the game)
    Move move(State state);
    Move move(State state, std::vector<Move> moves);
};

#endif /* A_Player_hpp */
