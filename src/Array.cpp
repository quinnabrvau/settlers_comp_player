//
//  Array.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/11/19.
//

#include "Array.hpp"
#ifdef TESTING
#include "unity.h"
#include <iostream>

template <class T>
void test__gen_array(void) {
    Shape s;
    for (int i = 1; i < 20; i++) {
        for(int j = 1; j < i; j++) {
            Array<T> A(i,j);
            s = A.shape();
            TEST_ASSERT_TRUE(s.first==i && s.second==j);
            TEST_ASSERT_TRUE(A==0);
            TEST_ASSERT_FALSE(A==1);
            TEST_ASSERT_TRUE(A==A);
            
            Array<T>B = Array_Zeros<T>(i,j);
            s = B.shape();
            TEST_ASSERT_TRUE(s.first==i && s.second==j);
            TEST_ASSERT_TRUE(B==0);
            TEST_ASSERT_FALSE(B==1);
            TEST_ASSERT_TRUE(A==B && B==A);
            
            B = Array_Zeros<T>(j,i);
            s = B.shape();
            TEST_ASSERT_TRUE(s.first==j && s.second==i);
            TEST_ASSERT_TRUE(B==0);
            TEST_ASSERT_FALSE(B==1);
            TEST_ASSERT_FALSE(A==B);
            TEST_ASSERT_FALSE(B==A);
            
            Array<T> C = Array_Ones<T>(i,j);
            s = C.shape();
            TEST_ASSERT_TRUE(s.first==i && s.second==j);
            TEST_ASSERT_FALSE(C==0);
            TEST_ASSERT_TRUE(C==1);
            TEST_ASSERT_FALSE(A==C);
            TEST_ASSERT_FALSE(A==B);
        }
    }
}

template <class T>
void test__multiply(void) {
    Shape s, a, b;
    for (int i = 1; i < 20; i++) {
        for(int j = 1; j < i; j++) {
            for (int k = 1; k < j; k++) {
                Array<T> A = Array_Ones<T>(i,j);
                Array<T> B = Array_Ones<T>(j,k);
                a = A.shape(); b = B.shape();
                TEST_ASSERT_TRUE(A.can_multiply(B));
                Array<T> C = A*B;
                s = C.shape();
                TEST_ASSERT_EQUAL(i, s.first);
                TEST_ASSERT_EQUAL(k, s.second);
                TEST_ASSERT_TRUE(C==j);
                
                A = Array_Zeros<T>(i,j);
                C = A*B;
                s = C.shape();
                TEST_ASSERT_EQUAL(i, s.first);
                TEST_ASSERT_EQUAL(k, s.second);
                TEST_ASSERT_TRUE(C==0);
                
                B = Array_Ones<T>(k,j);
                TEST_ASSERT_FALSE(A.can_multiply(B));
            }
        }
    }
}

template <class T, class K>
void test__multiply_tk(void) {
    Shape s, a, b;
    for (int i = 1; i < 20; i++) {
        for(int j = 1; j < i; j++) {
            for (int k = 1; k < j; k++) {
                Array<T> A = Array_Ones<T>(Shape(i,j));
                Array<K> B = Array_Ones<K>(Shape(j,k));
                a = A.shape(); b = B.shape();
                TEST_ASSERT_TRUE(A.can_multiply(B));
                Array<T> C = A*B;
                s = C.shape();
                TEST_ASSERT_EQUAL(i, s.first);
                TEST_ASSERT_EQUAL(k, s.second);
                TEST_ASSERT_TRUE(C == j);
                
                A = Array_Zeros<T>(i,j);
                C = A*B;
                s = C.shape();
                TEST_ASSERT_EQUAL(i, s.first);
                TEST_ASSERT_EQUAL(k, s.second);
                TEST_ASSERT_TRUE(C==0);
                
                B = Array_Ones<K>(k,j);
                TEST_ASSERT_FALSE(A.can_multiply(B));
            }
        }
    }
}

#define _NUM_T_RUN_TEST(func)           \
    do {                                \
    RUN_TEST( func <int> );             \
    RUN_TEST( func <short> );           \
    RUN_TEST( func <long> );            \
    RUN_TEST( func <unsigned int> );    \
    RUN_TEST( func <unsigned short> );  \
    RUN_TEST( func <unsigned long> );   \
    RUN_TEST( func <char> );            \
    RUN_TEST( func <float> );           \
    RUN_TEST( func <double> );          \
    RUN_TEST( func <long double> );     \
    } while(false)

typedef void (*void_func) (void);

#define _NUM_T_K_RUN_TEST(func)                             \
    do {                                                    \
    void_func _tk_func = func <int,short>;                  \
    RUN_TEST(_tk_func);                                     \
    _tk_func = func <int,float>;                            \
    RUN_TEST(_tk_func);                                     \
    _tk_func = func <float,double>;                         \
    RUN_TEST(_tk_func);                                     \
    _tk_func = func <float,int>;                            \
    RUN_TEST(_tk_func);                                     \
    } while(false)



void test__Array(void) {
    _NUM_T_RUN_TEST(test__gen_array);
    _NUM_T_RUN_TEST( test__multiply );
    _NUM_T_K_RUN_TEST( test__multiply_tk );
}
#endif//TESTING
