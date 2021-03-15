//
// Created by Kishan on 2021-03-15.
//
#include "Random.h"
#include <cstdlib>
#include <ctime>

int Random::RollDie(){
    srand((unsigned) time(0));
    int roll;
    roll = (rand() % maxValue) + minValue;
    return roll;
}

int Random::GenerateRandomNum(){
    srand((unsigned) time(nullptr));
    int rand;
    rand = (rand() % 42) + 1;
    return rand;
}