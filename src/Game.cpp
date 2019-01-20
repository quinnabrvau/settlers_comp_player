//
//  Game.cpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//

#include "Game.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> all_rolls() {
    int i, j;
    std::vector<int> rolls;
    for (i=1;i<=6;i++) {
        for (j=1;j<=6;j++) {
            rolls.push_back(i+j);
        }
    }
    return rolls;
}

Game::Game(int c_players, tRollType roll) : roll_type(roll) {
    _players.clear(); players.clear();
    for (int i = 0; i < c_players; i++) {
        _players.push_back(Player(i));
    }
    std::random_shuffle(_players.begin(), _players.end());
    for (int i = 0; i < c_players; i++) {
        players.push_back(&_players[i]);
    }
    board = Board(c_players);
}

Game::Game(std::vector<Player*> __players, tRollType roll) :
    roll_type(roll),
    players( __players.begin(), __players.end() ) {
    board = Board(__players.size());
}

void Game::start_phase(void) {
    int p ; //p for player
    for (p = 0; p < players.size(); p++) {
        start_turn(players[p], false);
    }
    for (p--; p >= 0; p--) {
        start_turn(players[p], true);
    }
}

void Game::start_turn(Player * player, bool get_resource) {
    std::vector<Move> moves;
    
    //Phase 1 build settlement
    std::vector<b_node*> nodes = board.get_available_settlements();
    for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
        if (get_resource) {
            moves.push_back(Move(m_place_settlement_get_resource, *it));
        } else {
            moves.push_back(Move(m_place_settlement, *it));
        }
    }
    Move move = player->move(moves);
    //check move in bounds
    ASSERT(move.first>m_start_moves_s && move.first<m_start_moves_e);
    // get the location where the settlement is tobe was placed
    b_node* node = (b_node*) move.second;
    // place settlement
    player->place_settlement(node, get_resource);
    
    //Phase 2 build road
    moves.clear();
    std::vector<b_edge*> edges = node->get_available_roads();
    for (auto it = edges.cbegin(); it != edges.cend(); it++) {
        moves.push_back(Move(m_place_road,*it));
    }
    move = player->move(moves);
    //check move in bounds
    ASSERT(move.first>m_start_moves_s && move.first<m_start_moves_e);
    // get the location where the settlement is tobe was placed
    b_edge* edge = (b_edge*) move.second;
    // place road
    edge->assign_road(player->get_player());
}

void Game::run_turn_move(Player * player, Move move) {
    ASSERT(move.first > m_turn_moves_s &&
           move.first < m_turn_moves_e);
    if (move.first == m_end_turn) return;
    switch (move.first) {
        case m_buy_settlement:
            player->buy_settlement((b_node*)move.second);
            break;
            
        case m_buy_city:
            player->buy_city((b_node*)move.second);
            break;
            
        case m_buy_road:
            player->buy_road((b_edge*)move.second);
            break;
            
        default:
            ASSERT(false);
            break;
    }
}

void Game::main_turn(Player * player) {
    //TODO give player option to play knight card
    
    int actions = 0;
    int roll = roll_dice();
    if (roll == 7) {
        rob_players();
        move_rober(player);
    } else {
        distribute_resources(roll);
    }
    
    Move move(m_start_move, NULL);
    while (move.first != m_end_turn &&
           actions++ < max_actions) {
        move = player->move();
        run_turn_move(player, move);
    };
}

void Game::rob_players(void) {
    //TODO
}


void Game::distribute_resources(int roll) {
    for (auto it = players.begin(); it != players.end(); it++) {
        (*it)->get_resources(roll);
    }
}
void Game::move_rober(Player *player){
    //choose location to move rober
    std::vector<Tile*> tiles = board.get_available_tiles_rober();
    std::vector<Move> moves;
    for (auto it = tiles.begin(); it != tiles.end(); it++) {
        moves.push_back(Move(m_move_rober,*it));
    }
    Move move = player->move(moves);
    // check that move is valid
    ASSERT(move.first == m_move_rober);
    
    // steal a card
    moves.clear();
    std::vector<int> stealable_players;
    stealable_players =  board.get_players_on_tile((Tile*)move.second);
    for (auto it = stealable_players.begin(); it != stealable_players.end(); it++) {
        if (*it != player->get_player()) {
            Player * t_player = players[*it];
            if (t_player->has_resources()) {
                moves.push_back(Move(m_steal_card, t_player));
            }
        }
    }
    if (moves.size()) {
        move = player->move(moves);
        Player * t_player = (Player*) move.second;
        int stolen_resource = t_player->steal();
        player->add_resource(stolen_resource);
    }
}

int Game::roll_dice(void) {
    int out = 0;
    if (roll_type == ROLL_DICE) {
        out = 2+(rand()%6)+(rand()%6);
    } else {
        if (stack.size()==0) { //create a new stack and shuffle the stack
            stack = all_rolls();
            std::random_shuffle(stack.begin(), stack.end());
        }
        if (roll_type == ROLL_STACK_5) { //remove the top five cards
            for (int i=0;i<5;i++) stack.pop_back();
        }
        // draw the top card
        out = stack.back();
        stack.pop_back();
    }
    return out;
}

int Game::is_winner() {
    for (int i = 0; i<players.size(); i++) {
        if (players[i]->points_pri() >= win_points) {
            return i;
        }
    }
    return -1;
}

int Game::play_game(void) {
    int winner = -1;
    int p = 0; //p for player
    turns = 0;
    std::cout << "Starting the game: " << players.size() << " players\n";
    start_phase();
    while (winner == -1) {
        std::cout << "Player: " << p << "'s turn: " << turns << "\n";
        main_turn(players[p]);
        winner = is_winner();
        p = (p+1)%players.size();
        turns++;
    }
    return winner;
}

std::vector<float> Game::eval_game(void) {
    std::vector<float> out(players.size());
    
    auto pit = players.begin();
    auto oit = out.begin();
    
    while (oit != out.end() &&
           pit != players.end()) {
        *oit++ = (*pit++)->points_pri() - turns*0.01f;
    }
    return out;
}

// TEST CODE
// GAME

#ifdef TESTING
#include "unity.h"

void test__Game_0(void) {
    Game game(4);
    game.play_game();
}
void test__Game_1(void) {
    Game game(3,ROLL_DICE);
    game.play_game();
}
void test__Game_2(void) {
    Game game(3,ROLL_STACK);
    game.play_game();
}
void test__Game_3(void) {
    Game game(3,ROLL_STACK_5);
    game.play_game();
}
void test__Game_4(void) {
    std::vector<Player> _players;
    std::vector<Player*> players;
    for (int i = 1; i<=4; i++) {
        _players.push_back(Player(i));
    }
    for (int i = 0; i<4; i++) {
        players.push_back(&_players[i]);
    }
    Game game4(players);
    game4.play_game();
}

void test__play__Game(void) {
    RUN_TEST(test__Game_0);
    RUN_TEST(test__Game_1);
    RUN_TEST(test__Game_2);
    RUN_TEST(test__Game_3);
    RUN_TEST(test__Game_4);
}

#endif//TESTING
