//
// Created by Kishan on 2021-03-15.
//

#ifndef FINALGAMEDESIGN_DICE_H
#define FINALGAMEDESIGN_DICE_H

// if player using 1 army, roll 1 die
// if player using 2 army, roll 2 die
// if player using > 3 army, roll
class Dice{
public:
    int RollDie();
    int roll;
    int maxValue = 6;
    int minValue = 1;

};

#endif //FINALGAMEDESIGN_DICE_H
