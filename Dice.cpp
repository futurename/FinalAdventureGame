//
// Created by Kishan on 2021-03-15.
//
#include "Dice.h"
#include <cstdlib>
#include <ctime>

int Dice::RollDie(){
    srand((unsigned) time(0));
    int roll;
    roll = (rand() % maxValue) + minValue;
    return roll;
}