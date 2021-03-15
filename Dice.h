//
// Created by Kishan on 2021-03-15.
//

#include "imports.h"

#ifndef FINALGAMEDESIGN_DICE_H
#define FINALGAMEDESIGN_DICE_H

// if player using 1 army, roll 1 die
// if player using 2 army, roll 2 die
// if player using > 3 army, roll
class Dice{
public:
    int RollDie();

    int GetMinValue();
    int GetMaxValue();


private:
    int roll;
    int min = 1;
    int max = 6;


};


#endif //FINALGAMEDESIGN_DICE_H
