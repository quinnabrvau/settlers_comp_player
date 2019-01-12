//
//  G_Player.hpp
//  GENETIC_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef G_Player_hpp
#define G_Player_hpp

#include "Genetic.hpp"
#include "Player.hpp"
#include <vector>

class G_Player:Player {
    std::vector<int> dims;
    std::vector<Genetic_Array<float>> g_array;
    
    Genetic_Array<float> calculate(Genetic_Array<float> state) {
        Genetic_Array<float> out = state;
        for (auto it = g_array.begin(); it != g_array.end(); it++) {
            out = out * (*it);
        }
        return out;
    }
    
public:
    G_Player(std::vector<int> _dims, int p = 0) :
        Player(p),
        dims(_dims.begin(),_dims.end()),
        g_array(dims.size()-1)
    {
        for (int i = 0; i < dims.size()-1; i++) {
            Genetic_Array<float> n_g_array(dims[i], dims[i+1]);
            g_array[i] = n_g_array;
        }
    }
    
    
    
};

#endif /* G_Player_hpp */
