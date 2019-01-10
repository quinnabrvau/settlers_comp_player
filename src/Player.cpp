//
//  Player.cpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//

#include "Player.hpp"
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <random>


void print_move(Move move) {
    std::cout << "\tMove: " << move.first << " @ " << move.second << "\n";
}
void print_moves(std::vector<Move> moves) {
    for (auto it = moves.begin(); it != moves.end(); it++) {
        std::cout << "\t";
        print_move(*it);
    }
}

int Player::points_pub() {
    int out = 0;
    for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
        b_node * node = *it;
        out += node->get_object();
    }
    return out;
}

int Player::points_pri() {
    int out = points_pub();
    // count victory point cards
    return out;
}

void Player::place_settlement(b_node * node, bool get_resources){
    nodes.push_back(node);
    node->assign_settlement(player);
    if (get_resources) {
        std::vector<int> res = node->get_resources();
        for (auto it = res.cbegin(); it != res.cend(); it++) {
            resources[*it]++;
        }
    }
}

bool Player::can_buy_settlement(void) {
    return (resources[r_wood]  >= 1 &&
            resources[r_sheep] >= 1 &&
            resources[r_brick] >= 1 &&
            resources[r_wheat] >= 1);
}
bool Player::can_buy_city(void) {
    return (resources[r_wheat] >= 2 && resources[r_ore]   >= 3);
}
bool Player::can_buy_road(void) {
    return (resources[r_wood]  >= 1 && resources[r_brick] >= 1);
}

void Player::buy_settlement(b_node * node){
    if ( can_buy_settlement() &&
         node->can_build() ) {
        resources[r_wood]--; resources[r_sheep]--; resources[r_brick]--; resources[r_wheat]--;
        node->assign_settlement(player);
        nodes.push_back(node);
    }
}
void Player::buy_city(b_node * node){
    if ( can_buy_city()  &&
        node->can_upgrade() ) {
        resources[r_wheat]-=2; resources[r_ore]-=3;
        node->upgrade_settlement();
    }
}
void Player::buy_road(b_edge * edge){
    if ( can_buy_road() &&
         edge->is_free() ) {
         resources[r_wood]--;  resources[r_brick]--;
         edge->assign_road(player);
    }
}

void Player::get_resources(int roll){
    for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
        std::vector<int> re = (*it)->get_resources(roll); //new resources
        for (auto rit = re.cbegin(); rit != re.cend(); rit++) {
            resources[*rit]++;
        }
    }
}

std::vector<b_node*> Player::get_available_settlements(void) {
    std::set<b_node*> set;
    for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
        std::vector<b_node*> av_nodes = (*it)->get_available_settlements();
        for (auto ait = av_nodes.cbegin(); ait != av_nodes.cend(); ait++) {
            set.insert(*ait);
        }
    }
    
    std::vector<b_node*> out(set.begin(), set.end());
    return out;
}

std::vector<b_edge*> Player::get_available_roads(void) {
    std::set<b_edge*> set;
    for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
        std::vector<b_edge*> av_nodes = (*it)->get_available_roads();
        for (auto ait = av_nodes.cbegin(); ait != av_nodes.cend(); ait++) {
            set.insert(*ait);
        }
    }
    std::vector<b_edge*> out(set.begin(), set.end());
    return out;
}

std::vector<b_node*> Player::get_available_cities(void) {
    std::vector<b_node*> out;
    for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
        if ((*it)->can_upgrade())
            out.push_back( *it );
    }
    return out;
}

std::vector<Move> Player::get_moves(void) {
    std::vector<Move> moves = {Move(m_end_turn,NULL),};
    if (can_buy_city()) {
        std::vector<b_node*> cities = get_available_cities();
        for (auto it = cities.cbegin(); it != cities.cend(); it++) {
            moves.push_back(Move(m_buy_city,*it));
        }
    }
    if (can_buy_road()) {
        std::vector<b_edge*> roads = get_available_roads();
        for (auto it = roads.cbegin(); it != roads.cend(); it++) {
            moves.push_back(Move(m_buy_road,*it));
        }
    }
    if (can_buy_settlement()) {
        std::vector<b_node*> setl = get_available_settlements();
        for (auto it = setl.cbegin(); it != setl.cend(); it++) {
            moves.push_back(Move(m_buy_settlement,*it));
        }
    }
    return moves;
}

int Player::_steal(void) {
    std::vector<unsigned int> all_res;
    int r, i;
    for ( r = R_START; r < R_END; r++){
        for ( i = 0; i < resources[r]; i++) {
            all_res.push_back(r);
        }
    }
    if (all_res.size()==0) return -1;
    return all_res[ uniform_dist( generator ) % all_res.size()];
}

int Player::steal(void) {
    int r = _steal();
    if (r==-1) return -1;
    resources[r]--;
    return r;
}
bool Player::has_resources(void) {
    for (int i = 0; i < R_END; i++) {
        if (resources[i]) return true;
    }
    return false;
}
int  Player::cnt_resources(void) {
    int out = 0;
    for (int i = 0; i < R_END; i++) {
        out += resources[i];
    }
    return out;
}

void Player::add_resource(int res) {
    ASSERT(res>=0 && res<R_END);
    resources[res]++;
}

//the basic player just chooses a random move
Move Player::move(std::vector<Move> moves) {
    if (moves.size() == 0) {
        return Move(m_no_moves,NULL);
    }
    int m =  uniform_dist( generator ) % moves.size();
    return moves[m];
}
Move Player::move(void) {
    std::vector<Move> moves = get_moves();
    return move(moves);
}

#ifdef TESTING
#include "unity.h"

void Player::test__clear_res(void) {
    for (int i = 0; i<R_END; i++) {
        resources[i] = 0;
    }
}
void Player::test__add_road_res(void) {
    add_resource(r_wood); add_resource(r_brick);
}
void Player::test__add_settlement_res(void) {
    test__add_road_res();
    add_resource(r_wheat); add_resource(r_sheep);
}
void Player::test__add_city_res(void) {
    add_resource(r_wheat);add_resource(r_wheat);
    add_resource(r_ore);add_resource(r_ore);
    add_resource(r_ore);
}
void Player::test__add_dev_res(void) {
    add_resource(r_ore);
    add_resource(r_wheat);
    add_resource(r_sheep);
}

std::vector<unsigned int> Player::test__get_resources(void) {
    std::vector<unsigned int> out(R_END);
    for (int i = 0; i<R_END; i++) {
        out[i] = resources[i];
    }
    return out;
}

void test__place_moves(void) {
    Player P(0);
    int i;
    std::vector<b_node> nodes(3);
    std::vector<b_edge> edges(2);
    std::vector<Move> moves;
    std::vector<Tile> tiles(4);
    for ( i = 0; i < tiles.size(); i++) {
        tiles[i].type = t_wood;
    }
    for ( i = 0; i < 2; i++) {
        nodes[0].tiles.push_back(&tiles[i]);
    }
    for ( ; i < tiles.size(); i++) {
        nodes[2].tiles.push_back(&tiles[i]);
    }
    for ( i = 0; i < edges.size(); i++) {
        nodes[i].adjacent.push_back(b_adj(&edges[i],&nodes[i+1]));
        nodes[i+1].adjacent.push_back(b_adj(&edges[i],&nodes[i]));
    }
    P.place_settlement(&nodes[0], false);
    TEST_ASSERT_FALSE(P.has_resources());
    TEST_ASSERT_EQUAL(0, P.cnt_resources());
    
    P.place_settlement(&nodes[2], true);
    TEST_ASSERT_TRUE(P.has_resources());
    TEST_ASSERT_EQUAL(2, P.cnt_resources());
}

void test__get_moves(void) {
    Player P(0);
    b_node*  p_node; b_edge* p_edge;
    std::vector<b_node> nodes(3);
    std::vector<b_edge> edges(2);
    std::vector<Move> moves;
    for (int i = 0; i < edges.size(); i++) {
        nodes[i].adjacent.push_back(b_adj(&edges[i],&nodes[i+1]));
        nodes[i+1].adjacent.push_back(b_adj(&edges[i],&nodes[i]));
    }
    P.place_settlement(&nodes[0], false);
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());

    P.test__add_road_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_road, moves[1].first);
    TEST_ASSERT_EQUAL_PTR(&edges[0],moves[1].second);

    P.test__clear_res();P.test__add_city_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_city, moves[1].first);
    TEST_ASSERT_EQUAL_PTR(&nodes[0],moves[1].second);

    P.test__clear_res();P.test__add_settlement_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_road, moves[1].first);
    TEST_ASSERT_EQUAL_PTR(&edges[0],moves[1].second);

    P.buy_road(moves[1].second);
    p_edge = (b_edge*) moves[1].second;
    TEST_ASSERT_FALSE(p_edge->is_free());
    TEST_ASSERT_TRUE(p_edge->is_player(0));

    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());

    P.test__add_road_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_road, moves[1].first);
    TEST_ASSERT_EQUAL_PTR(&edges[1],moves[1].second);

    P.test__clear_res();P.test__add_city_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_city, moves[1].first);
    TEST_ASSERT_EQUAL_PTR(&nodes[0],moves[1].second);

    P.test__clear_res();P.test__add_settlement_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_road, moves[1].first);
    TEST_ASSERT_EQUAL_PTR(&edges[1],moves[1].second);

    P.buy_road(moves[1].second);
    p_edge = (b_edge*) moves[1].second;
    TEST_ASSERT_FALSE(p_edge->is_free());
    TEST_ASSERT_TRUE(p_edge->is_player(0));
    
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());

    P.test__clear_res(); P.test__add_road_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());

    P.test__clear_res();P.test__add_city_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_city, moves[1].first);
    TEST_ASSERT_EQUAL_PTR(&nodes[0],moves[1].second);

    P.test__clear_res();P.test__add_settlement_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_settlement, moves[1].first);
    TEST_ASSERT_EQUAL_PTR(&nodes[2],moves[1].second);

    P.buy_settlement(moves[1].second);
    p_node = (b_node*) moves[1].second;
    TEST_ASSERT_EQUAL(s_settlement, p_node->get_object());
    TEST_ASSERT_EQUAL(0, p_node->get_player());

    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());

    P.test__add_road_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());
    P.test__clear_res();P.test__add_settlement_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());


    P.test__clear_res();P.test__add_city_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(3,moves.size());
    TEST_ASSERT_EQUAL(m_buy_city, moves[1].first);
    TEST_ASSERT_EQUAL(m_buy_city, moves[2].first);

    P.buy_city(moves[1].second);
    p_node = (b_node*) moves[1].second;
    TEST_ASSERT_EQUAL(s_city, p_node->get_object());
    TEST_ASSERT_EQUAL(0, p_node->get_player());

    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());

    P.test__clear_res();P.test__add_city_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(2,moves.size());
    TEST_ASSERT_EQUAL(m_buy_city, moves[1].first);

    P.buy_city(moves[1].second);
    p_node = (b_node*) moves[1].second;
    TEST_ASSERT_EQUAL(s_city, p_node->get_object());
    TEST_ASSERT_EQUAL(0, p_node->get_player());

    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());
    P.test__clear_res();P.test__add_city_res();
    moves = P.get_moves();
    TEST_ASSERT_EQUAL(m_end_turn, moves[0].first);
    TEST_ASSERT_EQUAL(1,moves.size());

      
}

void test__move(void) {
    Player P(0);
    std::vector<Move> moves = {
        Move(m_turn_moves_s,NULL), 
        Move(m_buy_settlement,NULL),
        Move(m_buy_city,NULL),  };
    std::vector<int> res = {0,0,0};
    for (int i = 0; i < 50000; i++) {
        Move move = P.move(moves);
        res[move.first]++;
    }
    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < i; j++) {
            TEST_ASSERT(res[i]*2>res[j] && res[j]*2>res[i]);
        }
    }
    moves.clear();
    Move move = P.move(moves);
    TEST_ASSERT_EQUAL(m_no_moves, move.first);
    TEST_ASSERT_EQUAL(NULL, move.second);
}

unsigned int test__sum(std::vector<unsigned int> in) {
    unsigned int out = 0;
    for (auto it = in.cbegin(); it != in.cend(); it++) {
        out += *it;
    }
    return out;
}

void test__get_resources(void) {
    Player P(0);
    int i;
    std::vector<b_node> nodes(3);
    std::vector<b_edge> edges(2);
    std::vector<Move> moves;
    std::vector<Tile> tiles(4);
    std::vector<unsigned int> res;

    for ( i = 0; i < 2; i++) {
        nodes[0].tiles.push_back(&tiles[i]);
        tiles[i].type = t_wood;
        tiles[i].roll = i+2;
        
    }
    for ( ; i < tiles.size(); i++) {
        nodes[2].tiles.push_back(&tiles[i]);
        tiles[i].type = t_ore;
        tiles[i].roll = i+2;
    }
    for ( i = 0; i < edges.size(); i++) {
        nodes[i].adjacent.push_back(b_adj(&edges[i],&nodes[i+1]));
        nodes[i+1].adjacent.push_back(b_adj(&edges[i],&nodes[i]));
    }
    P.place_settlement(&nodes[0], false);
    TEST_ASSERT_FALSE(P.has_resources());
    TEST_ASSERT_EQUAL(0, P.cnt_resources());
    
    for ( i = 0; i<2; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(1, test__sum(res));
        TEST_ASSERT_EQUAL(1, res[r_wood]);
    }
    for ( ; i<=10; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(0, test__sum(res));
    }
    
    P.test__add_city_res();
    P.buy_city(&nodes[0]);
    for ( i = 0; i<2; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(2, test__sum(res));
        TEST_ASSERT_EQUAL(2, res[r_wood]);
    }
    for ( ; i<=10; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(0, test__sum(res));
    }
    
    P.test__add_road_res();
    P.buy_road(&edges[0]);
    P.test__add_road_res();
    P.buy_road(&edges[1]);
    P.test__add_settlement_res();
    P.buy_settlement(&nodes[2]);
    
    for ( i = 0; i<2; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(2, test__sum(res));
        TEST_ASSERT_EQUAL(2, res[r_wood]);
    }
    for ( ; i<4; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(1, test__sum(res));
        TEST_ASSERT_EQUAL(1, res[r_ore]);
    }
    for ( ; i<=10; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(0, test__sum(res));
    }
    
    P.test__add_city_res();
    P.buy_city(&nodes[2]);
    
    for ( i = 0; i<2; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(2, test__sum(res));
        TEST_ASSERT_EQUAL(2, res[r_wood]);
    }
    for ( ; i<4; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(2, test__sum(res));
        TEST_ASSERT_EQUAL(2, res[r_ore]);
    }
    for ( ; i<=10; i++) {
        P.test__clear_res();
        P.get_resources(i+2);
        res = P.test__get_resources();
        TEST_ASSERT_EQUAL(0, test__sum(res));
    }
}
void test__steal(void) {
    Player P(0);
    P.test__steal();
}
void Player::test__steal(void) {
    for (int i = 0; i < R_END; i++) {
        for (int K = 1; K < 10; K++) {
            test__clear_res();
            TEST_ASSERT_EQUAL(-1, _steal());
            TEST_ASSERT_EQUAL(-1, steal());
            for (int k = 0; k < K; k++) {
                add_resource(i);
            }
            TEST_ASSERT_EQUAL(i, _steal());
            TEST_ASSERT_EQUAL(i, steal());
        }
    }
    for (int i = 0; i < R_END; i++) {
        for (int j = 0; j < R_END; j++) {
            for (int K = 1; K < 10; K++) {
                test__clear_res();
                TEST_ASSERT_EQUAL(-1, _steal());
                TEST_ASSERT_EQUAL(-1, steal());
                for (int k = 0; k < K; k++) {
                    add_resource(i);
                    add_resource(j);
                }
                int s = _steal();
                TEST_ASSERT(i == s || j == s);
                s = steal();
                TEST_ASSERT(i == s || j == s);
            }
        }
    }
    
}

void test__points(void) {
    Player P(0);
    int i;
    std::vector<b_node> nodes(3);
    std::vector<b_edge> edges(2);
    std::vector<Move> moves;
    std::vector<Tile> tiles(4);
    std::vector<unsigned int> res;
    
    for ( i = 0; i < 2; i++) {
        nodes[0].tiles.push_back(&tiles[i]);
        tiles[i].type = t_wood;
        tiles[i].roll = i+2;
        
    }
    for ( ; i < tiles.size(); i++) {
        nodes[2].tiles.push_back(&tiles[i]);
        tiles[i].type = t_ore;
        tiles[i].roll = i+2;
    }
    for ( i = 0; i < edges.size(); i++) {
        nodes[i].adjacent.push_back(b_adj(&edges[i],&nodes[i+1]));
        nodes[i+1].adjacent.push_back(b_adj(&edges[i],&nodes[i]));
    }
    
    TEST_ASSERT_EQUAL(0, P.points_pub());
    TEST_ASSERT_EQUAL(0, P.points_pri());
    
    P.place_settlement(&nodes[0], false);
    
    TEST_ASSERT_EQUAL(1, P.points_pub());
    TEST_ASSERT_EQUAL(1, P.points_pri());
    
    
    P.test__add_city_res();
    P.buy_city(&nodes[0]);
    
    TEST_ASSERT_EQUAL(2, P.points_pub());
    TEST_ASSERT_EQUAL(2, P.points_pri());
    
    
    P.test__add_road_res();
    P.buy_road(&edges[0]);
    P.test__add_road_res();
    P.buy_road(&edges[1]);
    P.test__add_settlement_res();
    P.buy_settlement(&nodes[2]);
    
    TEST_ASSERT_EQUAL(3, P.points_pub());
    TEST_ASSERT_EQUAL(3, P.points_pri());
    
    
    P.test__add_city_res();
    P.buy_city(&nodes[2]);
    
    TEST_ASSERT_EQUAL(4, P.points_pub());
    TEST_ASSERT_EQUAL(4, P.points_pri());
}


void test__player(void) {
    RUN_TEST(test__get_moves);
    RUN_TEST(test__move);
    RUN_TEST(test__place_moves);
    RUN_TEST(test__get_resources);
    RUN_TEST(test__steal);
    RUN_TEST(test__points);
}

#endif
