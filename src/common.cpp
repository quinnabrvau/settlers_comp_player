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
    assert(player==-1);
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
    assert(player == -1); assert(object==0);
    player=_player; object=s_settlement;
};
void b_node::upgrade_settlement(void) {
    assert(player != -1); assert(object==1);
    object=s_city;
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
    std::vector<b_node*> searched = {this};
    std::set<b_node*> set = {this};
    std::vector<b_node*> out;
    for (int i = 0; i < searched.size(); i++) {
        b_node* node = searched[i];
        if ( node->can_build() ) {
            // check that node is available and add it to output vector
            out.push_back(node);
        }
        for (auto ait = node->adjacent.begin(); ait != node->adjacent.end(); ait++) {
            // check that the current road is the current players
            if ( (*ait).first->is_player(player) ) {
                // check that we haven't visited that node yet
                if (set.find((*ait).second) == set.end()) {
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
    std::vector<b_node*> searched = {this};
    std::set<b_node*> set = {this};
    std::vector<b_edge*> out;
    for (int i = 0; i < searched.size(); i++) {
        b_node* node = searched[i];
        for (auto ait = node->adjacent.begin(); ait != node->adjacent.end(); ait++) {
            // check that the current road is the current players
            if ( (*ait).first->is_player(player) ) {
                // check that no other player owns the next node
                // check that we haven't visited that node yet
                if (((*ait).second->get_player() == get_player() ||
                    (*ait).second->get_player() == -1 ) &&
                    set.find((*ait).second) == set.end() ) {
                        searched.push_back((*ait).second);
                        set.insert((*ait).second);
                }
            } else if ((*ait).first->is_free()) {
                // check that the current road is free and add it to output vector
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
void b_node_test__assign_settlement(void) {
    b_node node;
    node.test__assign_settlement();
}
void b_node_test__upgrade_settlement(void) {
    b_node node;
    node.test__upgrade_settlement();
}
void b_node_test__has_port(void) {
    b_node node;
    node.test__has_port();
}
void b_node_test__get_resources(void) {
    b_node node;
    Tile tiles[3];
    for (int i = 0; i<3; i++) node.tiles.push_back(tiles+i);
    node.assign_settlement(0);
    node.test__get_resources(); // test with settlement
    node.upgrade_settlement();
    node.test__get_resources(); // test with city
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
    object = s_none;
    TEST_ASSERT_FALSE(can_upgrade());
    object = s_settlement;
    TEST_ASSERT_TRUE(can_upgrade());
    object = s_city;
    TEST_ASSERT_FALSE(can_upgrade());
}

void b_node::test__assign_settlement(void) {
    TEST_ASSERT_EQUAL(s_none, object);
    for (int p = 0; p < MAX_PLAYERS; p++) {
        player = -1; object=s_none;
        assign_settlement(p);
        TEST_ASSERT_EQUAL(s_settlement, object);
        TEST_ASSERT_EQUAL(p, player);
    }
}

void b_node::test__upgrade_settlement(void) {
    for (int p = 0; p < MAX_PLAYERS; p++) {
        object = s_settlement; player = p;
        upgrade_settlement();
        TEST_ASSERT_EQUAL(s_city, object);
        TEST_ASSERT_EQUAL(p, player);
    }
}

void b_node::test__has_port(void) {
    int Port = P_START;
    port = NULL;
    TEST_ASSERT_EQUAL(-1, has_port());
    port = &Port;
    for (Port = P_START; Port < P_END; Port++) {
        TEST_ASSERT_EQUAL(Port, has_port());
    }
}
void b_node::test__get_resources(void){
    std::vector<int> res; int i, j, r;
    std::vector<Tile*> a_tiles(tiles.begin(), tiles.end()); //all tiles for testing
    tiles.clear();
    res = get_resources();
    TEST_ASSERT_EQUAL(0, res.size());
    i = 2, r = R_START;
    for (auto it = a_tiles.begin(); it != a_tiles.end(); it++) {
        (*it)->roll = i++; (*it)->type = r++; (*it)->rob = false;
    }
    for (auto it = a_tiles.begin(); it != a_tiles.end(); it++) {
        tiles.clear();
        TEST_ASSERT_EQUAL(0,tiles.size());
        tiles.push_back(*it);
        TEST_ASSERT_EQUAL(1,tiles.size());
        res = get_resources();
        TEST_ASSERT_EQUAL(object, res.size());
        for (i = 0; i < object; i++) {
            TEST_ASSERT_EQUAL((*it)->type, res[i]);
        }
        r = (*it)->roll-1;
        res = get_resources(r);
        TEST_ASSERT_EQUAL(0, res.size());
        r = (*it)->roll;
        res = get_resources(r);
        TEST_ASSERT_EQUAL(object, res.size());
        for (i = 0; i < object; i++) {
            TEST_ASSERT_EQUAL((*it)->type, res[i]);
        }
        r = (*it)->roll+1;
        res = get_resources(r);
        TEST_ASSERT_EQUAL(0, res.size());
    }
    tiles.clear();
    for (i=0;i<a_tiles.size();i++) {
        tiles.push_back(a_tiles[i]);
        TEST_ASSERT_EQUAL(i+1, tiles.size());
        res = get_resources();
        TEST_ASSERT_EQUAL(object*(i+1), res.size());
        r = a_tiles[i]->roll;
        res = get_resources(r);
        TEST_ASSERT_EQUAL(object, res.size());
        for (j = 0; j < object; j++) {
            TEST_ASSERT_EQUAL(a_tiles[i]->type, res[j]);
        }
    }
    for (auto it = a_tiles.begin(); it != a_tiles.end(); it++) {
        (*it)->roll = 2;
    }
    res = get_resources(1);
    TEST_ASSERT_EQUAL(0, res.size());
    res = get_resources(2);
    TEST_ASSERT_EQUAL(object*a_tiles.size(), res.size());
    res = get_resources(3);
    TEST_ASSERT_EQUAL(0, res.size());
}
void b_node_test__get_available_settlements(void) {
    std::vector<b_edge> edges(4);
    std::vector<b_node> nodes(edges.size()+1);
    std::vector<b_node*> p_nodes;
    
    nodes[0].assign_settlement(0);
    std::vector<b_node*> a_nodes;
    a_nodes = nodes[0].get_available_settlements();
    TEST_ASSERT_EQUAL(0, a_nodes.size());
    for (int i = 0; i < edges.size(); i++) {
        edges[i].assign_road(0);
        nodes[i].adjacent.push_back(b_adj(&edges[i],&nodes[i+1]));
        nodes[i+1].adjacent.push_back(b_adj(&edges[i],&nodes[i]));
        a_nodes = nodes[0].get_available_settlements();
        TEST_ASSERT_EQUAL(i, a_nodes.size());
    }
    for (int i = 0; i < nodes.size(); i++) {
        p_nodes.push_back(&nodes[i]);
    }
    nodes[0].test__get_available_settlements(p_nodes);
}
void b_node::test__get_available_settlements(std::vector<b_node*> nodes) {
    std::vector<b_node*> a_nodes;
    for (int i=0;i<MAX_PLAYERS;i++) {
        nodes[2]->assign_settlement(i);
        a_nodes = nodes[0]->get_available_settlements();
        TEST_ASSERT_EQUAL(1, a_nodes.size());
        nodes[2]->test__clear();
        a_nodes = nodes[0]->get_available_settlements();
        TEST_ASSERT_EQUAL(3, a_nodes.size());
        
        nodes[3]->assign_settlement(i);
        a_nodes = nodes[0]->get_available_settlements();
        TEST_ASSERT_EQUAL(0, a_nodes.size());
        nodes[3]->test__clear();
        a_nodes = nodes[0]->get_available_settlements();
        TEST_ASSERT_EQUAL(3, a_nodes.size());
    }
    
}
void b_node_test__get_available_roads(void) {
    std::vector<b_edge> edges(4);
    std::vector<b_node> nodes(edges.size()+1);
    std::vector<b_node*> p_nodes;
    std::vector<b_edge*> p_edges;
    
    nodes[0].assign_settlement(0);
    std::vector<b_edge*> a_edges;
    a_edges = nodes[0].get_available_roads();
    TEST_ASSERT_EQUAL(0, a_edges.size());
    for (int i = 0; i < edges.size(); i++) {
        p_edges.push_back(&edges[i]);
        nodes[i].adjacent.push_back(b_adj(&edges[i],&nodes[i+1]));
        nodes[i+1].adjacent.push_back(b_adj(&edges[i],&nodes[i]));
        a_edges = nodes[0].get_available_roads();
        TEST_ASSERT_EQUAL(1, a_edges.size());
    }
    for (int i = 0; i < nodes.size(); i++) {
        p_nodes.push_back(&nodes[i]);
    }
    nodes[0].test__get_available_roads(p_nodes, p_edges);
}
void b_node::test__get_available_roads(std::vector<b_node*> nodes,
                                       std::vector<b_edge*> edges){
    std::vector<b_edge*> a_edges;
    int p = 0, i;
    
    for (i=0; i<edges.size()-1;i++) {
        edges[i]->assign_road(player);
        a_edges = get_available_roads();
        TEST_ASSERT_EQUAL(1, a_edges.size());
        for (p=0;p<MAX_PLAYERS;p++) {
            nodes[i+1]->assign_settlement(p);
            a_edges = get_available_roads();
            if (p==player) {
                TEST_ASSERT_EQUAL(1, a_edges.size());
            } else {
                TEST_ASSERT_EQUAL(0, a_edges.size());
            }
            nodes[i+1]->test__clear();
        }
    }
    //assign the last road to a different player
    edges[i]->assign_road(player+1);
    a_edges = get_available_roads();
    TEST_ASSERT_EQUAL(0, a_edges.size());
}

void test__board_node(void) {
    RUN_TEST(b_node_test__get_object);
    RUN_TEST(b_node_test__get_player);
    RUN_TEST(b_node_test__can_build);
    RUN_TEST(b_node_test__can_upgrade);
    RUN_TEST(b_node_test__assign_settlement);
    RUN_TEST(b_node_test__upgrade_settlement);
    RUN_TEST(b_node_test__has_port);
    RUN_TEST(b_node_test__get_resources);
    RUN_TEST(b_node_test__get_available_settlements);
    RUN_TEST(b_node_test__get_available_roads);
}
