#include "Country.h"
#include "HelperFunctions/ColorList.h"

string Country::getCountryName() {
    return countryName;
}

void Country::setCountryName(string inCountryName) {
    countryName = inCountryName;
}

string Country::getContinentName() {
    return continentName;
}

void Country::setContinentName(string inContinentName) {
    continentName = inContinentName;
}

tuple<int, int, int, int> Country::getCountryColour() {
    return countryColour;
}

void Country::setCountryColour(tuple<int, int, int, int> inCountryColour) {
    countryColour = inCountryColour;
}

Country::Country(string name, int x, int y, vector<string> &countries) {
    this->countryName = name;
    this->coordinateX = x;
    this->coordinateY = y;
    this->countryColour = ColorList::GREEN;
    this->numOfArmy = 3;
    vector<string> adjacentCountries;
    for (string str: countries) {
        adjacentCountries.push_back(str);
    }
    this->adjacentCountries = adjacentCountries;
}

int Country::getX() {
    return coordinateX;
}

int Country::getY() {
    return coordinateY;
}

vector<string> &Country::getAdjacentCountires() {
    return adjacentCountries;
}

Country::Country() {
}

int Country::getCountryArmy() {
    return numOfArmy;
}

tuple<int, int, int, int> Country::getTextColor() {
    return textColor;
}

void Country::loseOneArmy() {
    --numOfArmy;
}

int Country::getOwnerIndex() {
    return ownerIndex;
}

void Country::setOwnerIndex(int index) {
    ownerIndex = index;
}
