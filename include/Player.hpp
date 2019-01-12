//
//  Player.hpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <vector>
#include <algorithm>
#include <random>
#include "b_common.hpp"

typedef enum {
    // error moves:
    m_no_moves = -100,
    m_start_move,
    
    //valid game moves:
    
    //turn moves
    m_turn_moves_s = 0,
    m_buy_settlement,
    m_buy_city,
    m_buy_road,
    m_buy_dev_card,
    
    m_end_turn,
    m_turn_moves_e,
    
    //start game moves
    m_start_moves_s,
    m_place_settlement,
    m_place_settlement_get_resource,
    m_place_road,
    m_start_moves_e,
    
    // special moves to move the rober
    m_move_rober,
    m_steal_card,
} tMoves;

typedef std::pair<tMoves,void *> Move;
void print_move(Move move);
void print_moves(std::vector<Move> moves);

class Player {
    std::vector<unsigned int> resources;
    std::vector<b_node*> nodes; //heads of the two colonies
    //TODO add developement cards
    int player;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> uniform_dist;
    int _steal(void);
public:
    Player(int p) : resources(5), player(p) {
        uniform_dist = std::uniform_int_distribution<int>(0, 200000);
        for (int i = 0; i < R_END; i++)
            resources[i] = 0;
    };
    
    int get_player(void) {return player;}
    // gets the points the oponents can see
    int points_pub();
    // gets the points the player can see
    int points_pri();
    
    // places a settlement
    // used at the begining of the game
    // allows for different start conditions to be set
    void place_settlement(b_node * node, bool get_resources=false);
    
    // buys an object at a location
    // (does some checks if move is allowed) (not exaustive)
    // removes the correct number of resources and places the object
    void buy_settlement(b_node * node);
    void buy_city(b_node * node);
    void buy_road(b_edge * edge);

    void buy_settlement(void * node) {buy_settlement((b_node*)node);}
    void buy_city(void * node) {buy_city((b_node*)node);}
    void buy_road(void * edge) {buy_road((b_edge*)edge);}
    
    //checks if the player gets resources on a roll
    void get_resources(int roll);
    
    //checks for all locations to build a current settlement
    std::vector<b_node*> get_available_settlements(void);
    std::vector<b_node*> get_available_cities(void);
    std::vector<b_edge*> get_available_roads(void);
    
    //check if the resources exist to buy items
    bool can_buy_settlement(void);
    bool can_buy_city(void);
    bool can_buy_road(void);
    
    
    
    //rober steals card
    int steal(void);
    // checks if a player has any resources
    bool has_resources(void);
    int  cnt_resources(void);
    
    // allows for the giving of resources, like a trade or steal
    void add_resource(int);
    
    //get regular game moves
    std::vector<Move> get_moves(void);
    
    // take a turn
    // given a set of moves (think start the game)
    
    //For higher level classes define:
//    Move move(State state);
//    Move move(State state, std::vector<Move> moves);
    
    //For basic testing of game mechanics
    Move move(std::vector<Move> moves);
    // a normal turn
    Move move(void);
    
    //TESTS
    //Helper functions
    void test__clear_res(void);
    void test__add_road_res(void);
    void test__add_settlement_res(void);
    void test__add_city_res(void);
    void test__add_dev_res(void);
    std::vector<unsigned int> test__get_resources(void);
    void test__steal(void);
};

void test__player(void);

#endif /* Player_hpp */
