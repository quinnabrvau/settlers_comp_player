//
//  common.cpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//


#include "common.hpp"
#include "unity.h"
#include <iostream>


// BOARD EDGE
void b_edge::assign_road(int _player) {
    ASSERT(player==-1);
    player=_player;
};
bool b_edge::is_player(int _player) { return player==_player; };
bool b_edge::is_free(void) {return player==-1;};


// BOARD NODE
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
    ASSERT(player == -1); ASSERT(object==0);
    player=_player; object=1;
};
void b_node::upgrade_settlement(void) {
    ASSERT(player != -1); ASSERT(object==1);
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
    ASSERT(object>0); ASSERT(player>=0);
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
    ASSERT(object>0); ASSERT(player>=0);
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
            } else if ((*ait).first->is_free()) {
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


// TEST CODE
// BOARD EDGE

void b_edge_test_assign_road(void) {
    b_edge edge;
    edge.test__assign_road();
}
void b_edge_test__is_player(void) {
    b_edge edge;
    edge.test__is_player();
}
void b_edge_test__is_free(void) {
    b_edge edge;
    edge.test__is_free();
}
void test__board_edge(void) {
    RUN_TEST(b_edge_test_assign_road);
    RUN_TEST(b_edge_test__is_player);
    RUN_TEST(b_edge_test__is_free);
}
void b_edge::test__assign_road(void) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        player = -1;
        assign_road(i);
        TEST_ASSERT_EQUAL(player, i);
    }
}
void b_edge::test__is_player(void) {
    for (int j = -1; j < MAX_PLAYERS; j++) {
        player = j;
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (i==j) {
                TEST_ASSERT_TRUE( is_player(i) );
            } else {
                TEST_ASSERT_FALSE( is_player(i) )
            }
        }
    }
}
void b_edge::test__is_free(void) {
    player = -1;
    TEST_ASSERT_TRUE(is_free());
    for (int j = 0; j < MAX_PLAYERS; j++) {
        player = j;
        TEST_ASSERT_FALSE(is_free());
    }
}

// BOARD NODE
void b_node_test__get_object(void) {
    b_node node;
    node.test__get_object();
}
void b_node_test__get_player(void) {
    b_node node;
    node.test__get_player();
}
void b_node_test__can_build(void) {
    b_node nodes[4];
    b_edge edges[3];
    for (int i = 0; i < 3; i++) {
        nodes[0].adjacent.push_back(b_adj(edges+i,nodes+i));
    }
    nodes[0].test__can_build();
}
void b_node_test__can_upgrade(void) {
    b_node node;
    node.test__can_upgrade();
}
void test__board_node(void) {
    RUN_TEST(b_node_test__get_object);
    RUN_TEST(b_node_test__get_player);
    RUN_TEST(b_node_test__can_build);
}

void b_node::test__get_object(void) {
    for (object = 0; object < S_END; object++) {
        TEST_ASSERT_EQUAL(object, get_object());
    }
}
void b_node::test__get_player(void) {
    for (player = 0; player < MAX_PLAYERS; player++) {
        TEST_ASSERT_EQUAL(player, get_player());
    }
}
void b_node::test__can_build(void) {
    TEST_ASSERT_TRUE(can_build());
    for (object = s_none+1; object < S_END; object++) {
        TEST_ASSERT_FALSE(can_build());
    }
    object = s_none;
    for (auto it = adjacent.begin(); it != adjacent.end(); it++) {
        TEST_ASSERT_TRUE(can_build());
        for (int i = s_none+1; i < S_END; i++) {
            (*it).second->object = i;
            TEST_ASSERT_FALSE(can_build());
        }
        (*it).second->object = s_none;
    }
    TEST_ASSERT_TRUE(can_build());
    for (auto it = adjacent.begin(); it != adjacent.end(); it++) {
        for (int i = s_none+1; i < S_END; i++) {
            (*it).second->object = i;
            TEST_ASSERT_FALSE(can_build());
        }
    }
}

void b_node::test__can_upgrade(void) {
    
}
