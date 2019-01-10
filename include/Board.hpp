//
//  Board.hpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <utility>
#include <vector>
#include <random>
#include <cassert>

#include "b_common.hpp"

class Board {
private:
    std::vector<b_edge>              edges;
    std::vector<std::vector<Tile>>   tiles;
    std::vector<std::vector<b_node>> nodes;
    int n_corners = 2*(6+6+1)+(6*3+4);
    int n_edges   = 2*(6*2+4)+(6*3+8)+2*7;
    int players = 4;
    void build_board(void);
    void gen_sets(void);
public:
    void clear(void);

    Board(int _players=4) : players(_players) {
        ASSERT(_players>0);
        clear();
        gen_sets();
        build_board();
    }
    
    
    
    void print_board(void);
    
    std::vector<b_node*> get_available_settlements(void);
    std::vector<Tile*> get_available_tiles_rober(void);
    Tile* get_current_tile_rober(void);
    std::vector<int> get_players_on_tile(Tile * tile);
};
//TODO : Assign ports
#endif /* Board_hpp */
