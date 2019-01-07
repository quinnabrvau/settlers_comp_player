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
        std::vector<b_node*> roads = get_available_settlements();
        for (auto it = roads.cbegin(); it != roads.cend(); it++) {
            moves.push_back(Move(m_buy_city,*it));
        }
    }
    if (can_buy_settlement()) {
        std::vector<b_node*> setl = get_available_settlements();
        for (auto it = setl.cbegin(); it != setl.cend(); it++) {
            moves.push_back(Move(m_buy_city,*it));
        }
    }
    return moves;
}

int Player::steal(void) {
    int range = 0, i, card;
    for ( i = 0; i < R_END; i++) {
        range += resources[i];
    }
    if (!range) return -1;
    card = rand()%range;
    range = 0;
    for ( i = 0; i < R_END && range < card; i++) {
        range += resources[i];
    }
    if (i==R_END) i = R_END-1;
    resources[i]--;
    return i;
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
    assert(res>=0 && res<R_END);
    resources[res]++;
}

//the basic player just chooses a random move
Move Player::move(std::vector<Move> moves) {
    if (moves.size() == 0) {
        return Move(m_no_moves,NULL);
    }
    int m = rand()%moves.size();
    print_moves(moves);
    print_move(moves[m]);
    return moves[m];
}
Move Player::move(void) {
    std::vector<Move> moves = get_moves();
    return move(moves);
}
