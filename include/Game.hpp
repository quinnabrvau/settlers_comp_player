//
//  Game.hpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp
#include "Player.hpp"
#include "Board.hpp"
#include "b_common.hpp"


typedef enum {
    ROLL_DICE,
    ROLL_STACK,
    ROLL_STACK_5,
} tRollType;

class Game {
    tRollType roll_type = ROLL_DICE;
    Board board;
    std::vector<Player> _players;
    std::vector<Player*> players;
    std::vector<int> stack; // used for drawing from a stack of cards
    int max_actions = 100;
    int win_points = 10;
public:
    Game(int c_players = 4, tRollType roll = ROLL_DICE);
    Game(std::vector<Player*> __players, tRollType roll = ROLL_DICE);
    
    void start_phase(void);
    void start_turn(Player * player, bool get_resource=false);
    
    void main_turn(Player * player);
    void run_turn_move(Player * player, Move move);
    
    void move_rober(Player * player);
    void rob_players(void);
    
    int is_winner(void);
    
    int roll_dice(void);
    void distribute_resources(int roll);
    
    int play_game(void);
};

void test__play__Game(void);

#endif /* Game_hpp */
