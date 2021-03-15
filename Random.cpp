#include "Random.h"
#include <cstdlib>
#include <ctime>

int Random::RollDie(int low, int high){
    return GenerateRandomNum(low, high);
}

int Random::GenerateRandomNum(int low, int high){
    srand((unsigned) time(nullptr));
    return rand() % (high - low) + low;
}