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
                A = Array_Ones<T>(Shape(i,j));
                C = A*2;
                TEST_ASSERT(C==2);
                TEST_ASSERT(B==1);
                
                C*=2;
                TEST_ASSERT(C==4);
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
            
            C = A+1;
            TEST_ASSERT(A==0);
            TEST_ASSERT(C==1);
            
            C+=1;
            TEST_ASSERT(C==2);
            
            C-=1;
            TEST_ASSERT(C==1);
            
            C = C-1;
            TEST_ASSERT(C==0);
        }
    }
}

void test__Array(void) {
    _NUM_T_RUN_TEST(test__gen_array);
    _NUM_T_K_RUN_TEST(test__multiply_tk);
    _NUM_T_K_RUN_TEST(test__add_tk);
}

#endif//TESTING
