//
//  Array.hpp
//  TEST
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef Array_h
#define Array_h

#include <vector>
#include <utility>
#include "common.hpp"

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
    
    
    Shape output_shape(Array *A) {
        Shape s = this->shape();
        Shape sa = A->shape();
        return Shape(s.first, sa.second);
    }
    bool can_multiply(Array* A) {
        Shape s = this->shape();
        Shape sa = A->shape();
        return s.second==sa.first;
    }
    void static_multiply(Array *A, Array*B) {
        Shape s = this->shape();
        Shape sa = A->shape();
        Shape sb = B->shape();
        ASSERT(sb == output_shape(A)); // check output shape
        ASSERT(s.second == sa.first); // check middle dimensions
        //TODO multiply
        for (int i = 0; i < s.first; i++) {
            for (int j = 0; j < sa.second; j++) {
                T* t = &B->array[i][j];
                *t = 0;
                for (int k = 0; k<s.second; k++) {
                    *t += this->array[i][k] * A->array[k][j];
                }
            }
        }
    }
    
    Array operator*(Array* A) {
        ASSERT(can_multiply(A));
        Array B(output_shape(A));
        static_multiply(A, &B);
        return B;
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
    void operator*=(float f) {
        for (auto rit = array.begin(); rit != array.end(); rit++) {
            for (auto cit = *rit.begin(); cit != *rit.end(); cit++) {
                *cit *= f;
            }
        }
    }
    void operator*=(int i) {
        this*=((float)i);
    }
};

#endif /* Array_h */
