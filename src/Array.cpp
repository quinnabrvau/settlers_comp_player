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

template <class T, class K>
void test__add_tk(void) {
    Shape s, a, b;
    for (int i = 1; i < 20; i++) {
        for(int j = 1; j < i; j++) {
            Array<T> A = Array_Ones<T>(Shape(i,j));
            Array<K> B = Array_Ones<K>(Shape(i,j));
            a = A.shape(); b = B.shape();
            TEST_ASSERT_TRUE(A.can_add(B));
            Array<T> C = A+B;
            s = C.shape();
            TEST_ASSERT(s == A.shape());
            TEST_ASSERT_TRUE(C == 2);
            TEST_ASSERT_TRUE(C != 1);
            TEST_ASSERT_TRUE(C != 0);
            
            A = Array_Zeros<T>(i,j);
            C = A+B;
            s = C.shape();
            TEST_ASSERT(s == A.shape());
            TEST_ASSERT_TRUE(C == 1);
            TEST_ASSERT_TRUE(C != 2);
            TEST_ASSERT_TRUE(C != 0);
            
            B = Array_Zeros<K>(j,i);
            TEST_ASSERT_FALSE(A.can_add(B));
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

typedef unsigned int unint;
typedef unsigned short unshort;
typedef unsigned long unlong;
typedef long long longlong;
typedef long double longdouble;

#define ___RUN_TEST(func,type1,type2)       \
    do {                                    \
    void_func func ## _ ## type1 ## _ ## type2      \
     = func < type1, type2 >;                       \
    RUN_TEST( (func ## _ ## type1 ## _ ## type2) ); \
    } while(false)

#define RUN_TEST_S(func, type1)                \
    do {                                       \
    ___RUN_TEST( func,type1,int );             \
    ___RUN_TEST( func,type1,short );           \
    ___RUN_TEST( func,type1,long );            \
    ___RUN_TEST( func,type1,longlong );        \
    ___RUN_TEST( func,type1,unint );           \
    ___RUN_TEST( func,type1,unshort );         \
    ___RUN_TEST( func,type1,unlong );          \
    ___RUN_TEST( func,type1,char );            \
    ___RUN_TEST( func,type1,float );           \
    ___RUN_TEST( func,type1,double );          \
    ___RUN_TEST( func,type1,longdouble );      \
    } while(false)

#define _NUM_T_K_RUN_TEST(func)          \
    do {                                 \
    RUN_TEST_S( func, int );             \
    RUN_TEST_S( func, short );           \
    RUN_TEST_S( func, long );            \
    RUN_TEST_S( func, longlong );        \
    RUN_TEST_S( func, unint );           \
    RUN_TEST_S( func, unshort );         \
    RUN_TEST_S( func, unlong );          \
    RUN_TEST_S( func, char );            \
    RUN_TEST_S( func, float );           \
    RUN_TEST_S( func, double );          \
    RUN_TEST_S( func, longdouble );      \
    } while(false)



void test__Array(void) {
    _NUM_T_RUN_TEST(test__gen_array);
    _NUM_T_K_RUN_TEST(test__multiply_tk);
    _NUM_T_K_RUN_TEST(test__add_tk);
}

#endif//TESTING
