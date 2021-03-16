#include "ColorList.h"
using namespace std;

const tuple<int, int, int, int> ColorList::RED{255, 0, 0, 1};
const tuple<int, int, int, int> ColorList::GREEN{0, 255, 0, 1};
const tuple<int, int, int, int> ColorList::BLUE{0, 0, 255, 1};
const tuple<int, int, int, int> ColorList::YELLOW{255, 255, 0, 1};
const tuple<int, int, int, int> ColorList::WHITE{255, 255, 255, 1};
const tuple<int, int, int, int> ColorList::BLACK{0, 0, 0, 1};
const tuple<int, int, int, int> ColorList::LIGHTER_YELLOW{255, 255, 240, 1};

vector<tuple<int, int, int, int>> playerColors = {ColorList::RED,
                                                  ColorList::GREEN,
                                                  ColorList::BLUE,
                                                  ColorList::YELLOW,
                                                  ColorList::WHITE};

vector<tuple<int, int, int, int>> TextColors = {  ColorList::BLACK,
                                                  ColorList::LIGHTER_YELLOW,
                                                  ColorList::WHITE,
                                                  ColorList::BLACK,
                                                  ColorList::GREEN};


void ColorList::generateColorPairs(int numOfPlayers){
    for (size_t i = 0; i < numOfPlayers; i++){
        pair <tuple<int, int, int, int>, tuple<int, int, int, int>> colorPair;
        colorPair = make_pair(playerColors.at(i), TextColors.at(i));
        colorPairs.push_back(colorPair);
    }
}



