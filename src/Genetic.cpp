//
//  Genetic.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#include "Genetic.hpp"
#include <random>

template<class T>
void Genetic_Array<T>::mutate(float mutation_rate) {
    Shape s = this->shape();
    int size = s.first*s.second;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0, size);
    dist.reset();
    int changes = size * mutation_rate;
    while (changes-- > 0) {
        int r = dist(generator);
        this->array[r%s.first][r/s.first] *= mutation_rate;
    }
}

template<class T>
void Genetic_Array<T>::_breed(Genetic_Array& G ) {
    Shape s = this->shape(); Shape t = G.shape();
    ASSERT(s==t);
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0, 1);
    dist.reset();
    Genetic_Array<T> out(s);
    for (int i = 0; i < s.first; i++) {
        for (int j = 0; j < s.second; j++) {
            if (dist(generator)) {
                out(i,j) = G(i,j);
            } else {
                out(i,j) = this(i,j);
            }
        }
    }
    return out;
}

template<class T>
Genetic_Array<T> Genetic_Array<T>::bread(Genetic_Array<T>& G, float mutation_rate) {
    Genetic_Array<T> out = _breed(G);
    out.mutate(mutation_rate);
    return out;
}
template<class T>
void Genetic_Array<T>::random(void) {
    std::default_random_engine generator;
    std::uniform_real_distribution<T> dist(-10000,10000);
    for (auto rit = this->array.begin(); rit != this->array.end(); rit++) {
        for (auto cit = *rit.begin(); cit != *rit.end(); cit++) {
            *cit = dist(generator);
        }
    }
}
