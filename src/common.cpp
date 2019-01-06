//
//  common.cpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//


#include "common.hpp"
#include <iostream>

void b_edge::assign_road(int _player) {
    assert(player==-1);
    player=_player;
};
bool b_edge::is_player(int _player) { return player==_player; };
bool b_edge::available_road(void) {return player==-1;};


int b_node::get_object(void) {return object;}
int b_node::get_player(void) {return player;}

bool b_node::can_build(void) {
    if (object!=s_none) return false;
    for (auto it = adjacent.cbegin(); it != adjacent.cend(); it++) {
        if ((*it).second->object != s_none) return false;
    }
    return true;
};
bool b_node::can_upgrade(void) {return object==s_settlement;};

void b_node::assign_settlement(int _player) {
    assert(player == -1); assert(object==0);
    player=_player; object=1;
};
void b_node::upgrade_settlement(void) {
    assert(player != -1); assert(object==1);
    object=2;
};
int  b_node::has_port(void) {if (port==NULL) return -1; return *port;};

std::vector<int> b_node::get_resources(void) {
    std::vector<int> out;
    if (object==0 || player==-1) {
        return out;
    }
    for (auto it = tiles.cbegin(); it != tiles.cend(); it++) {
        out.push_back((*it)->type);
        if (object==s_city) {
            out.push_back((*it)->type);
        }
    }
    return out;
}

std::vector<int> b_node::get_resources(int roll) {
    std::vector<int> out;
    if (object==0 || player==-1) {
        return out;
    }
    for (auto it = tiles.cbegin(); it != tiles.cend(); it++) {
        if ((*it)->roll == roll && (*it)->rob == false) {
            out.push_back((*it)->type);
            if (object==s_city) {
                out.push_back((*it)->type);
            }
        }
    }
    return out;
}

std::vector<b_node*> b_node::get_available_settlements() {
    assert(object>0); assert(player>=0);
    std::list<b_node*> searched = {this};
    std::set<b_node*> set = {this};
    std::vector<b_node*> out;
    for (auto it = searched.cbegin(); it != searched.cend(); it++) {
        if ( (*it)->can_build() ) {
            out.push_back(*it);
        }
        for (auto ait = adjacent.cbegin(); ait != adjacent.cend(); ait++) {
            if ( (*ait).first->is_player(player) ) {
                if (set.find((*ait).second) != set.end()) {
                    searched.push_back((*ait).second);
                    set.insert((*ait).second);
                }
            }
        }
    }
    return out;
}

// iteratively checks for all available roads attached
// to a given node
std::vector<b_edge*> b_node::get_available_roads() {
    assert(object>0); assert(player>=0);
    std::list<b_node*> searched = {this};
    std::set<b_node*> set = {this};
    std::vector<b_edge*> out;
    for (auto it = searched.cbegin(); it != searched.cend(); it++) {
        for (auto ait = adjacent.cbegin(); ait != adjacent.cend(); ait++) {
            if ( (*ait).first->is_player(player) ) {
                if (set.find((*ait).second) != set.end()) {
                    searched.push_back((*ait).second);
                    set.insert((*ait).second);
                }
            } else if ((*ait).first->available_road()) {
                out.push_back((*ait).first);
            }
        }
    }
    return out;
}

void b_node::print_node(void) {
    std::cout << "\tplayer: " << player << "\tobject: " << object << "\n";
    std::cout << "\tconected edge->node(" <<adjacent.size() << "): " ;
    for (auto it = adjacent.cbegin(); it != adjacent.cend(); it++) {
        b_adj adj = *it;
        std::cout << adj.first << "->" << adj.second << "\t";
    } std::cout << "\n";
    std::cout << "\tconected tiles(" <<tiles.size() << "): ";
    for (auto it = tiles.cbegin(); it != tiles.cend(); it++) {
        std::cout << (*it)->type << " " << (*it)->roll << "\t";
    } std::cout << "\n";
}


