//
//  Genetic.hpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef Genetic_hpp
#define Genetic_hpp
#include "Array.hpp"

template<class T>
class Genetic_Array:Array<T> {
private:
    void _mutate(float mutation_rate = 0.0);
    void _breed(Genetic_Array* G);
public:
    Genetic_Array bread(Genetic_Array* G, float mutation_rate = 0.0);
};

#endif /* Genetic_hpp */
