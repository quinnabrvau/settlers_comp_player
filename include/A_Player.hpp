//
//  A_Player.hpp
//  GENETIC_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef A_Player_hpp
#define A_Player_hpp

#include "Matrix.hpp"
#include "Player.hpp"
#include <vector>

typedef matrix::Matrix<float> State;
typedef matrix::Matrix<float> pl_matrix;

class A_Player:public Player {
    std::vector<int> dims;
    std::vector<pl_matrix> array;
    
    pl_matrix calculate(State state);
    
public:
    A_Player(std::vector<pl_matrix> _array, int p = 0);
    
    // take a turn
    // given a set of moves (think start the game)
    Move move(State state);
    Move move(State state, std::vector<Move> moves);
};

#endif /* A_Player_hpp */
