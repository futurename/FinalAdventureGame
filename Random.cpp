#include "Random.h"
#include <cstdlib>
#include <ctime>

pair<int, int> Random::RollDie(int numOfDices, int low, int high) {
    vector<int> numbers;
    if (numOfDices == 1) {
        return pair<int, int>{GenerateRandomNum(low, high), 0};
    }
    if (numOfDices == 2) {
        numbers.push_back(GenerateRandomNum(low, high));
        numbers.push_back(GenerateRandomNum(low, high));
        sort(numbers.begin(), numbers.end(),[&](int&a, int&b){return a > b;});
        return pair<int,int>{numbers.at(0), numbers.at(1)};
    }
    numbers.push_back(GenerateRandomNum(low, high));
    numbers.push_back(GenerateRandomNum(low, high));
    numbers.push_back(GenerateRandomNum(low, high));
    sort(numbers.begin(), numbers.end(),[&](int&a, int&b){return a > b;});
    return pair<int,int>{numbers.at(0), numbers.at(1)};
}

int Random::GenerateRandomNum(int low, int high) {

    return rand() % (high - low + 1) + low;
}