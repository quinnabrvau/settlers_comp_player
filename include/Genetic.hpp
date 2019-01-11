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
class Genetic_Array:public Array<T> {
    void _breed(Genetic_Array& G);
public:
    Genetic_Array(const Genetic_Array& G)  {
        Shape s = G.shape();
        this->array(s.first);
        for (int i = 0; i<s.first; i++) {
            this->array[i] = std::vector<T>(G.array[i].cbegin(), G.array[i].cend());
        }
    }
    Genetic_Array(Shape s) : Array<T>(s) {random();}
    Genetic_Array(int cols = 1, int rows = 1) : Array<T>(cols,rows) {random();}
    
    
    
    void random(void);
    void mutate(float mutation_rate = 0.0);
    Genetic_Array bread(Genetic_Array& G, float mutation_rate = 0.0);
};

#endif /* Genetic_hpp */
