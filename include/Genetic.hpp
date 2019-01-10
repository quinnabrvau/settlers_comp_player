//
//  Genetic.hpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef Genetic_hpp
#define Genetic_hpp
#include <vector>
#include <utility>

#ifndef ASSERT
    #ifdef TESTING
        #include "unity.h"
        #define ASSERT(...) TEST_ASSERT(__VA_ARGS__)
    #else
        #include <cassert>
        #define ASSERT(...) assert(__VA_ARGS__)
    #endif//TESTING
#endif//ASSERT

typedef std::pair<int,int> Shape;
typedef Shape Position;

template<class T>
class Array {
    std::vector<std::vector<T>> array;
    void init_array(int rows = 1, int cols = 1) {
        array.clear();
        for (int r = 0; r<rows; r++) {
            array.push_back(std::vector<T>(cols));
        }
    };
public:
    Array(Shape s) {
        init_array(s.first, s.second);
    }
    Array(int rows = 1, int cols = 1) {
        init_array(rows,cols);
    };
    Array copy(void) {
        Shape s = shape();
        Array A(s);
        for (int i = 0; i<s.first; i++) {
            A.array[i] = std::vector<T>(array[i].begin(), array[i].end());
        }
    }
    int size(void) {return array.size();}
    Shape shape(void) {
        int s = size();
        if (s>0)
            return std::pair<int,int>(s,array[0].size());
        else
            return std::pair<int,int>(0,0);
    }
    
    T operator[](Position index) {return array[index.first][index.second];};
    std::vector<T> operator[](int row) { return array[row]; }
    
    
    bool can_multiply(Array* A) {
        Shape s = this->shape();
        Shape sa = A->shape();
        return s.second==sa.first;
    }
    Array operator*(Array* A) {
        Shape s = this->shape();
        Shape sa = A->shape();
        ASSERT(can_multiply(A));
        Array B(Shape(s.first,sa.second));
    }
    Array operator*(float f) {
        Array A = copy();
        for (auto rit = A.array.begin(); rit != A.array.end(); rit++) {
            for (auto cit = *rit.begin(); cit != *rit.end(); cit++) {
                *cit *= f;
            }
        }
    }
    Array operator*(int i) {
        return this*((float)i);
    }
};

template<class T>
class Genetic_Array:Array<T> {
private:
    void _mutate(float mutation_rate = 0.0);
    void _breed(Genetic_Array* G);
public:
    Genetic_Array bread(Genetic_Array* G, float mutation_rate = 0.0);
};

#endif /* Genetic_hpp */
