//
//  A_Player.cpp
//  GENETIC_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#include "A_Player.hpp"

A_Player::A_Player(std::vector<Array<float>> _array, int p) :
    Player(p), array(_array.begin(), _array.end()) { }


Array<float> A_Player::calculate(State state) {
    Array<float> out = state;
    auto it = array.begin();
    out = state * (*it);
    for (it++; it != array.end(); it++) {
        //TODO add tanh function
        out = out * (*it);
    }
    return out;
}


Move A_Player::move(State state) {
    std::vector<Move> moves = get_moves();
    return move(state, moves);
}
Move A_Player::move(State state, std::vector<Move> moves) {
    if (moves.size() == 0) {
        return Move(m_no_moves,NULL);
    }
    if (moves.size() == 1) {
        return moves[0];
    }
    Array<float> weights = calculate(state);
    
    // sort list of all possible moves by weights
    
    // iterate through list, find first move in currently allowed moves
    
    // return first move
    
    
    return Move(m_no_moves,NULL);
}



