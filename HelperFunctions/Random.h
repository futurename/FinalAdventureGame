#include "imports.h"

class Random {
public:
    static pair<int, int> RollDie(int numOfDices, int low = 1, int high = 6);

    static int generateRandomNum(int low, int high);
};