//
//  A_Player.cpp
//  GENETIC_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#include "A_Player.hpp"

using namespace matrix;

A_Player::A_Player(std::vector<pl_matrix> _array, int p) :
    Player(p), array(_array.begin(), _array.end()) { }
    
A_Player::A_Player (iterator be, iterator end, int p) : Player(p) {
        array.clear();
        while (be != end) {
            array.push_back( pl_matrix( be.begin(), be.end() ) );
            be*=1;
        }
    }

pl_matrix A_Player::calculate(State state) {
    pl_matrix out = state;
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
    pl_matrix weights = calculate(state);
    
    // sort list of all possible moves by weights
    
    // iterate through list, find first move in currently allowed moves
    
    // return first move
    
    
    return Move(m_no_moves,NULL);
}



