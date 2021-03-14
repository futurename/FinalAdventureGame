#include "ColorList.h"

tuple<int, int, int, int> ColorList::RED{255, 0, 0, 1};
tuple<int, int, int, int> ColorList::GREEN{0, 255, 0, 1};
tuple<int, int, int, int> ColorList::BLUE{0, 0, 255, 1};
tuple<int, int, int, int> ColorList::YELLOW{255, 255, 0, 1};

tuple<int, int, int, int> ColorList::getRed() {
    return RED;
}

tuple<int, int, int, int> ColorList::getGreen() {
    return GREEN;
}

tuple<int, int, int, int> ColorList::getBlue() {
    return BLUE;
}

tuple<int, int, int, int> ColorList::getYellow() {
    return YELLOW;
}
