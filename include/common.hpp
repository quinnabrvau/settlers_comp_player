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

#define DEFAULT_RESOURCES(x) x ## wood, x ## wheat, x ## brick, x ## ore, x ## sheep

typedef enum {
    DEFAULT_RESOURCES(r_),
    R_END,
} tResources;

typedef enum {
    DEFAULT_RESOURCES(p_),
    p_any,
    P_END,
} tPort;

typedef enum {
    DEFAULT_RESOURCES(t_),
    t_desert,
    T_END,
} tTile;

typedef enum {
    s_none = 0,
    s_settlement,
    s_city,
    S_END,
} tSettlement;

typedef struct {
    int type, roll;
    bool rob;
} Tile;

class b_edge{
private:
    int player = -1;
public:
    void assign_road(int _player) {
        assert(player==-1);
        player=_player;
    };
    bool is_player(int _player) { return player==_player; };
    bool available_road(void) {return player==-1;};
};

class b_node {
private:
    int player = -1, object = 0;
public:
    std::vector<std::pair< b_edge*,b_node* >> adjacent;
    std::vector<Tile*>   tiles;
    int * port = NULL;
    
    int get_object(void) {return object;}
    int get_player(void) {return player;}
    
    bool can_build(void) {
        if (object!=s_none) return false;
        for (auto it = adjacent.cbegin(); it != adjacent.cend(); it++) {
            if ((*it).second->object != s_none) return false;
        }
        return true;
    };
    bool can_upgrade(void) {return object==s_settlement;};
    
    void assign_settlement(int _player) {
        assert(player == -1); assert(object==0);
        player=_player; object=1;
    };
    void upgrade_settlement(void) {
        assert(player != -1); assert(object==1);
        object=2;
    };
    int  has_port(void) {if (port==NULL) return -1; return *port;};
    
    std::vector<int> get_resources(void) {
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
    
    std::vector<int> get_resources(int roll) {
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
    
    std::vector<b_node*> get_available_settlements() {
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
    std::vector<b_edge*> get_available_roads() {
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
    
    void print_node(void);
};
typedef std::pair< b_edge*,b_node* > b_adj;

#endif /* common_h */
