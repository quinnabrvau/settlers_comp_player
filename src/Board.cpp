//
//  Board.cpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//

#include "Board.hpp"
#include <random>
#include <algorithm>

#include <iostream>


std::vector<int> rolls_4_player() {
    std::vector<int> rolls;
    for (int i = 2; i<=11; i++) {
        if (i!=7)
            rolls.push_back(i);
        if (i+1!=7)
            rolls.push_back(i+1);
    }
    return rolls;
}

std::vector<int> board_4_player() {
    int i;
    std::vector<int> types;
    for (i=0;i<4;i++) {
        types.push_back(t_wood);
        types.push_back(t_sheep);
        types.push_back(t_wheat);
    }
    for (i=0;i<3;i++) {
        types.push_back(t_ore);
        types.push_back(t_brick);
    }
    types.push_back(t_desert);
    return types;
}

std::vector<int> tile_shape_4_player() {
    std::vector<int> out = { 3,4,5,4,3 };
    return out;
}

std::vector<int> node_shape_4_player() {
    std::vector<int> out = {7, 9, 11, 11, 9, 7};
    return out;
}

void Board::gen_sets(int seed) {
    if (seed != -1)
        srand(seed);
    this->gen_sets();
}

void Board::gen_sets(void) {
    int i, j, k;
    std::vector<int> node_shape;
    std::vector<int> tile_shape;
    std::vector<int> rolls;
    std::vector<int> types;
    if (players <= 4) {
        node_shape = node_shape_4_player();
        tile_shape = tile_shape_4_player();
        rolls = rolls_4_player();
        types = board_4_player();
    } else {
        assert(false);
    }
    assert(rolls.size()==types.size()-1);
    
    std::shuffle(types.begin(), types.end(), std::default_random_engine());
    std::shuffle(rolls.begin(), rolls.end(), std::default_random_engine());
    
    k = 0;
    for (i = 0; i<n_corners;k++) {
        std::vector<b_node> new_nodes;
        for (j=0;j<node_shape[k];j++) {
            b_node new_node;
            new_nodes.push_back(new_node);
            i++;
        }
        nodes.push_back(new_nodes);
    }
    k = 0;
    for (i = 0; i<n_edges;i++) {
        b_edge new_edge;
        edges.push_back(new_edge);
    }
    auto tit = types.cbegin();
    auto rit = rolls.cbegin();
    k = 0;
    while (rit != rolls.cend() && tit != types.cend()) {
        assert(k<tile_shape.size());
        std::vector<Tile> new_tiles;
        for (j=0;j<tile_shape[k];j++) {
            if (*tit != t_desert) {
                Tile new_tile = {.type=*tit++, .roll=*rit++, .rob=false};
                new_tiles.push_back(new_tile);
                
            } else {
                Tile new_tile = {.type=*tit++, .roll = 0, .rob=true};
                new_tiles.push_back(new_tile);
            }
        }
        tiles.push_back(new_tiles);
        k++;
    }
}

void Board::clear() {
    edges.clear(); tiles.clear(); nodes.clear();
}

void Board::build_board() {
    int i,j,k;
    // assign tiles to nodes
    for ( i = 0; i < tiles.size(); i++) {
        for ( j = 0; j <tiles[i].size(); j++ ) {
            for ( k = 0; k < 3; k++) {
                int a, b;
                if (i<2) {
                    a = j*2+k+0;
                    b = j*2+k+1;
                } else if (i==2) {
                    a = j*2+k+0;
                    b = j*2+k+0;
                } else {
                    a = j*2+k+1;
                    b = j*2+k+0;
                }
                nodes[i  ][a].tiles.push_back(&tiles[i][j]);
                nodes[i+1][b].tiles.push_back(&tiles[i][j]);
            }
        }
    }
    // assign edges to nodes
    k=0;
    
    for ( i = 0; i < nodes.size(); i++) {
        for ( j = 0; j <nodes[i].size(); j++ ) {
            if (j+1 < nodes[i].size()) {
                // add the next node to the right
                // there is no next node to the last node
                b_adj adj1(&edges[k],&nodes[i  ][j+1]);
                nodes[i  ][j  ].adjacent.push_back(adj1);
                b_adj adj2(&edges[k],&nodes[i  ][j  ]);
                nodes[i  ][j+1].adjacent.push_back(adj2);
            } k++;
            if (i < 2) {
                if (j%2==0) {
                    b_adj adj1(&edges[k],&nodes[i+1][j+1]);
                    nodes[i  ][j  ].adjacent.push_back(adj1);
                    b_adj adj2(&edges[k],&nodes[i  ][j  ]);
                    nodes[i+1][j+1].adjacent.push_back(adj2);
                }
            } else if (i == 2) {
                if (j%2==0) {
                    b_adj adj1(&edges[k],&nodes[i+1][j  ]);
                    nodes[i  ][j  ].adjacent.push_back(adj1);
                    b_adj adj2(&edges[k],&nodes[i  ][j  ]);
                    nodes[i+1][j  ].adjacent.push_back(adj2);
                }
            } else if (i+1<nodes.size()) {
                if (j%2==1) {
                    b_adj adj1(&edges[k],&nodes[i+1][j-1]);
                    nodes[i  ][j  ].adjacent.push_back(adj1);
                    b_adj adj2(&edges[k],&nodes[i  ][j  ]);
                    nodes[i+1][j-1].adjacent.push_back(adj2);
                }
            }
        }
    }
}

void Board::print_board(void) {
    int i, j;
    for ( i = 0; i < nodes.size(); i++) {
        for ( j = 0; j <nodes[i].size(); j++ ) {
            std::cout << "node: " << i << "," << j << "\n";
            nodes[i][j].print_node();
        }
    }
    for ( i = 0; i < tiles.size(); i++) {
        for ( j = 0; j <tiles[i].size(); j++ ) {
            std::cout << "tile: " << i << "," << j << "\t";
            std::cout << tiles[i][j].type << "," << tiles[i][j].roll << "," << tiles[i][j].rob << "\n";
        }
    }
}

std::vector<b_node*> Board::get_available_settlements(void) {
    std::vector<b_node*> out;
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        for (auto ait = (*it).begin(); ait != (*it).end(); ait++) {
            if ((*ait).can_build()) {
                out.push_back(&(*ait));
            }
        }
    }
    return out;
}

std::vector<Tile*> Board::get_available_tiles_rober(void) {
    std::vector<Tile*> out;
    for (auto it = tiles.begin(); it != tiles.end(); it++) {
        for (auto ait = (*it).begin(); ait != (*it).end(); ait++) {
            if (!(*ait).rob) {
                out.push_back(&(*ait));
            }
        }
    }
    return out;
}
Tile* Board::get_current_tile_rober(void) {
    for (auto it = tiles.begin(); it != tiles.end(); it++) {
        for (auto ait = (*it).begin(); ait != (*it).end(); ait++) {
            if ((*ait).rob) {
                return &(*ait);
            }
        }
    }
    return NULL;
}

std::vector<int> Board::get_players_on_tile(Tile * tile) {
    std::set<int> set;
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        for (auto ait = (*it).begin(); ait != (*it).end(); ait++) {
            if ((*ait).get_player() >= 0 &&
                std::find((*ait).tiles.begin(), (*ait).tiles.end(), tile) != (*ait).tiles.end()) {
                set.insert((*ait).get_player());
            }
        }
    }
    std::vector<int> out(set.begin(),set.end());
    return out;
}
