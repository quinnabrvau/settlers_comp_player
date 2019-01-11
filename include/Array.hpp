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
    void init_array(int cols = 1, int rows = 1) {
        array.clear();
        for (int c = 0; c<cols; c++) {
            array.push_back(std::vector<T>(rows));
        }
    };
public:
    Array(const Array& A) {
        Shape s = A.shape();
        array = std::vector<std::vector<T>>(s.first);
        for (int i = 0; i<s.first; i++) {
            array[i] = std::vector<T>(A.array[i].cbegin(), A.array[i].cend());
        }
    }
    Array(Shape s) {
        init_array(s.first, s.second);
    }
    Array(int cols = 1, int rows = 1) {
        init_array(cols,rows);
    };
    Array copy(void) {
        Shape s = shape();
        Array A(s);
        for (int i = 0; i<s.first; i++) {
            A.array[i] = std::vector<T>(array[i].begin(), array[i].end());
        }
    }
    
    int size(void) const {return array.size();}
    Shape shape(void) const{
        int s = size();
        if (s>0)
            return std::pair<int,int>(s,array[0].size());
        else
            return std::pair<int,int>(0,0);
    }
    
    T& operator[](Position index) {return array[index.first][index.second];};
    std::vector<T>& operator[](int col) { return array[col]; };
    T& operator()(int col, int row) {return array[col][row];};
    /*void operator=(Array* A) {
        this = A->copy();
    }*/
    template<class K>
    Shape output_shape(Array<K>& A) {
        Shape s = this->shape();
        Shape sa = A.shape();
        return Shape(s.first, sa.second);
    }
    template<class K>
    bool can_multiply(Array<K>& A) {
        Shape s = this->shape();
        Shape sa = A.shape();
        return s.second==sa.first;
    }
private:
    template<class K, class L>
    void static_multiply(Array<T>& A, Array<K>& B, Array<L>& out) {
        Shape sa = A.shape();
        Shape sb = B.shape();
        Shape sout = out.shape();
        ASSERT(sout == A.output_shape(B)); // check output shape
        ASSERT(sa.second == sb.first); // check middle dimensions
        //TODO multiply
        for (int i = 0; i < sa.first; i++) {
            for (int j = 0; j < sb.second; j++) {
                L* l = &out[i][j];
                *l = 0;
                for (int k = 0; k<sa.second; k++) {
                    *l += A(i,k)*B(k,j);
                }
            }
        }
    }
public:
    template<class K>
    Array operator*(Array<K>& A) {
        ASSERT(can_multiply(A));
        Array B(output_shape(A));
        static_multiply(*this,A,B);
        return B;
    }
    template<class K>
    Array operator*(K f) {
        Array A = *this;
        A *= f;
        return A;
    }
    template<class K>
    void operator*=(K f) {
        for (auto rit = array.begin(); rit != array.end(); rit++) {
            for (auto cit = *rit.begin(); cit != *rit.end(); cit++) {
                *cit *= f;
            }
        }
    }
    template<class K>
    bool can_add(Array<K>& A) {
        Shape s = this->shape();
        Shape sa = A->shape();
        if (s.first == sa.first) {
            return sa.second == 1 || s.second == 1 || sa.second == s.second;
        }
        if (s.second == sa.second) {
            return sa.first == 1 || s.first == 1;
        }
        if (s.first == 1 || s.second == 1) return true;
        return sa.first == 1 && sa.second == 1;
    }
private:
    template<class K>
    void static_add_row_vec(Array<K>& A, Array& B) {
        Shape s = this->shape();
        Shape sa = A->shape();
        Shape sb = B->shape();
        ASSERT(sb == s);
        ASSERT(s.second == sa.second && sa.first == 1);
        for (int c = 0; c < s.first; c++ ) {
            for (int r = 0; r < s.second; r++) {
                B->array[c][r] = array[c][r] + A->array[0][r];
            }
        }
    }
    template<class K>
    void static_add_col_vec(Array<K>& A, Array& B) {
        Shape s = this->shape();
        Shape sa = A->shape();
        Shape sb = B->shape();
        ASSERT(sb == s);
        ASSERT(s.first == sa.first && sa.second == 1);
        for (int c = 0; c < s.first; c++ ) {
            for (int r = 0; r < s.second; r++) {
                B->array[c][r] = array[c][r] + A->array[c][0];
            }
        }
    }
    template<class K>
    void static_add_array(Array<K>& A, Array& B) {
        Shape s = this->shape();
        Shape sa = A->shape();
        Shape sb = B->shape();
        ASSERT(sb == s && sb == sa);
        for (int c = 0; c < s.first; c++ ) {
            for (int r = 0; r < s.second; r++) {
                B->array[c][r] = array[c][r] + A->array[c][r];
            }
        }
    }
public:
    template<class K>
    void static_add(Array<K>& A, Array& B) {
        Shape s = this->shape();
        Shape sa = A->shape();
        Shape sb = B->shape();
        ASSERT(can_add(A));
        ASSERT(sb == s);
        if (sa == s) {
            static_add_array(A, B);
        } else if (sa.first == 1) {
            static_add_row_vec(A, B);
        } else if (sa.second == 1) {
            static_add_col_vec(A, B);
        } else {
            ASSERT(false);
        }
    }
    template<class K>
    Array operator+(Array<K>& A) {
        ASSERT(can_add(A));
        Array B(shape());
        static_add(A, &B);
        return B;
    }
    template<class K>
    void operator+=(Array<K>& A) {
        static_add(A, this);
    }
    template<class K>
    Array operator+(K f) {
        Array A = copy();
        for (auto rit = A.array.begin(); rit != A.array.end(); rit++) {
            for (auto cit = *rit.begin(); cit != *rit.end(); cit++) {
                *cit += f;
            }
        }
    }
    template<class K>
    void operator+=(K f) {
        for (auto rit = array.begin(); rit != array.end(); rit++) {
            for (auto cit = *rit.begin(); cit != *rit.end(); cit++) {
                *cit += f;
            }
        }
    }
    
    void operator-(void) {
        for (auto rit = array.begin(); rit != array.end(); rit++) {
            for (auto cit = *rit.begin(); cit != *rit.end(); cit++) {
                *cit = -*cit;
            }
        }
    }
    template<class K>
    Array operator-(K f) {
        return this+(-f);
    }
    template<class K>
    void operator-=(K f) {
        this += (-f);
    }
    
    
// Boolean Checks
    template<class K>
    bool operator==(Array<K>& A) {
        Shape s = shape(); Shape sa = A.shape();
        if (s.first != sa.first || s.second != sa.second) return false;
        for (int i = 0; i < s.first; i++) {
            for (int j = 0; j < s.second; j++) {
                if (array[i][j] != A(i,j)) return false;
            }
        }
        return true;
    }
    template<class K>
    bool operator==(K k) {
        Shape s = shape();
        for (int i = 0; i < s.first; i++) {
            for (int j = 0; j < s.second; j++) {
                if (array[i][j] != k) return false;
            }
        }
        return true;
    }
};


template <class T>
Array<T> Array_Zeros(Shape shape) {
    Array<T> A(shape);
    return A;
}
template <class T>
Array<T> Array_Zeros(int cols=1, int rows=1) {
    return Array_Zeros<T>(Shape(cols,rows));
}

template <class T>
Array<T> Array_Ones(Shape shape) {
    Array<T> A(shape);
    for (int i = 0; i < shape.first; i++) {
        for (int j = 0; j < shape.second; j++) {
            A(i,j) = 1;
        }
    }
    return A;
}
template <class T>
Array<T> Array_Ones(int cols=1, int rows=1) {
    return Array_Ones<T>(Shape(cols,rows));
}

void test__Array(void);

#endif /* Array_h */
