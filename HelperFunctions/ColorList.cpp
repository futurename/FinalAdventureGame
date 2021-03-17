#include "ColorList.h"

using namespace std;

const tuple<int, int, int, int> ColorList::RED{225, 50, 0, 1};
const tuple<int, int, int, int> ColorList::GREEN{50, 225, 0, 1};
const tuple<int, int, int, int> ColorList::BLUE{50, 50, 225, 1};
const tuple<int, int, int, int> ColorList::YELLOW{255, 255, 0, 1};
const tuple<int, int, int, int> ColorList::WHITE{255, 255, 255, 1};
const tuple<int, int, int, int> ColorList::BLACK{0, 0, 0, 1};
const tuple<int, int, int, int> ColorList::LIGHTER_YELLOW{255, 255, 240, 1};
const tuple<int, int, int, int> ColorList::PURPLE{237, 74, 246, 1};
const tuple<int, int, int, int> ColorList::CYAN{74, 246, 237, 1};

vector<pair<tuple<int, int, int, int>, tuple<int, int, int, int>>> ColorList::colorPairs{{ColorList::RED,    ColorList::BLUE},
                                                                                         {ColorList::GREEN,  ColorList::RED},
                                                                                         {ColorList::PURPLE,   ColorList::CYAN},
                                                                                         {ColorList::YELLOW, ColorList::BLUE},
                                                                                         {ColorList::PURPLE, ColorList::GREEN}};


vector<tuple<int, int, int, int>> playerColors = {ColorList::RED,
                                                  ColorList::GREEN,
                                                  ColorList::BLUE,
                                                  ColorList::YELLOW,
                                                  ColorList::WHITE};

vector<tuple<int, int, int, int>> TextColors = {ColorList::BLACK,
                                                ColorList::LIGHTER_YELLOW,
                                                ColorList::WHITE,
                                                ColorList::BLACK,
                                                ColorList::GREEN};


void ColorList::generateColorPairs(int numOfPlayers) {
    for (size_t i = 0; i < numOfPlayers; i++) {
        pair<tuple<int, int, int, int>, tuple<int, int, int, int>> colorPair;
        colorPair = make_pair(playerColors.at(i), TextColors.at(i));
        colorPairs.push_back(colorPair);
    }
}

vector<pair<tuple<int, int, int, int>, tuple<int, int, int, int>>> ColorList::getPlayerColorPairs() {
    return colorPairs;
}



