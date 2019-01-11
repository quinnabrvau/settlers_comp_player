//
//  Array.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/11/19.
//

#include "Array.hpp"
#ifdef TESTING
#include "unity.h"

template <class T>
void test__multiply(void) {
    Shape s;
    for (int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            for (int k = 0; k < 20; k++) {
                Array<T> A(i,j);
                Array<T> B(j,k);
                TEST_ASSERT_TRUE(A.can_multiply(B));
                Array<T> C = A*B;
                s = C.shape();
                TEST_ASSERT_EQUAL(i, s.first);
                TEST_ASSERT_EQUAL(j, s.second);
            }
        }
    }
}

#define _NUM_T_RUN_TEST(func)           \
    do {                                \
    RUN_TEST( func <int> );             \
    RUN_TEST( func <short> );           \
    RUN_TEST( func <long> );            \
    RUN_TEST( func <float> );           \
    RUN_TEST( func <double> );          \
    RUN_TEST( func <long double> );     \
    } while(false)


void test__Array(void) {
    _NUM_T_RUN_TEST( test__multiply );
}
#endif//TESTING
