//
//  genetic_test.cpp
//  GAME_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/11/19.
//

#include "Array.hpp"
#include "Genetic.hpp"

#include "unity.h"

int main(int argc, const char * argv[]) {
    UNITY_BEGIN();
    test__Array();
    test__Genetic();
    return UNITY_END();
}
