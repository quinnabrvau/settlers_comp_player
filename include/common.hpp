//
//  common.hpp
//  Catan_learner
//
//  Created by Quinn Abrahams-Vaughn on 1/5/19.
//  Copyright © 2019 Quinn Abrahams-Vaughn. All rights reserved.
//

#ifndef common_h
#define common_h

#include <vector>
#include <list>
#include <set>
#include <cassert>
#include <algorithm>

#ifdef TESTING
    #include "unity.h"
    #define ASSERT(...) TEST_ASSERT(__VA_ARGS__)
#else
    #define ASSERT(...) assert(__VA_ARGS__)
#endif

const int MAX_PLAYERS = 6;

#define DEFAULT_RESOURCES(x) x ## wood, x ## wheat, x ## brick, x ## ore, x ## sheep
#define FIRST_RESOURCE(x) x ## wood
typedef enum {
    DEFAULT_RESOURCES(r_),
    R_END,
} tResources;
#define R_START FIRST_RESOURCE(r_)
typedef enum {
    DEFAULT_RESOURCES(p_),
    p_any,
    P_END,
} tPort;
#define P_START FIRST_RESOURCE(p_)
typedef enum {
    DEFAULT_RESOURCES(t_),
    t_desert,
    T_END,
} tTile;
#define T_START FIRST_RESOURCE(t_)
typedef enum {
    s_none = 0,
    s_settlement,
    s_city,
    S_END,
} tSettlement;

class Tile {
public:
    int type, roll;
    bool rob = false;
    Tile(int _type, int _roll, bool _rob=false) :
        type(_type), roll(_roll), rob(_rob) {}
    Tile() :
    type(-1), roll(-1), rob(false) {}
};

class b_edge{
private:
    int player = -1;
public:
    void assign_road(int _player);
    bool is_player(int _player);
    bool is_free(void);
    
    void test__assign_road(void);
    void test__is_player(void);
    void test__is_free(void);
    void test__clear(void) {player=-1;}
};
void test__board_edge(void);

class b_node {
private:
    int player = -1, object = 0;
public:
    std::vector<std::pair< b_edge*,b_node* >> adjacent;
    std::vector<Tile*>   tiles;
    int * port = NULL;
    //TODO Assign ports
    
    int get_object(void);
    int get_player(void);
    
    bool can_build(void);
    bool can_upgrade(void);
    void assign_settlement(int _player);
    void upgrade_settlement(void);
    int  has_port(void);
    
    std::vector<int> get_resources(void);
    std::vector<int> get_resources(int roll);
    std::vector<b_node*> get_available_settlements();
    std::vector<b_edge*> get_available_roads();
    void print_node(void);
    
//test code
    void test__get_object(void);
    void test__get_player(void);
    void test__can_build(void);
    void test__can_upgrade(void);
    void test__assign_settlement(void);
    void test__upgrade_settlement(void);
    void test__has_port(void);
    void test__get_resources(void);
    void test__get_available_settlements(std::vector<b_node*> nodes);
    void test__get_available_roads(std::vector<b_node*> nodes,
                                   std::vector<b_edge*> edges);
    void test__clear(void) {player=-1;object=0;}
};

void test__board_node(void);

typedef std::pair< b_edge*,b_node* > b_adj;

#endif /* common_h */
