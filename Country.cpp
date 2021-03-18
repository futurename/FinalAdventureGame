#include "Country.h"
#include "HelperFunctions/ColorList.h"

string Country::getCountryName() {
    return countryName;
}

void Country::setCountryName(string inCountryName) {
    countryName = inCountryName;
    this->numOfArmy = 1;
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

Country::Country(string name, int x, int y, vector<string> &countries, int numOfArmy) {
    this->countryName = name;
    this->coordinateX = x;
    this->coordinateY = y;
    this->countryColour = ColorList::WHITE;
    this->numOfArmy = numOfArmy;
    this->ownerIndex = NO_COUNTRY_OWNER;
    vector<string> adjacentCountries;
    for (const string &str: countries) {
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

void Country::setTextColor(tuple<int, int, int, int> color) {
    this->textColor = color;
}


int Country::getNumOfArmy() {
    return numOfArmy;
}

void Country::setNumOfArmy(int newNumOfArmy) {
    numOfArmy = newNumOfArmy;
}

void Country::reduceNumOfArmy(int numOfArmy) {
    this->numOfArmy -= numOfArmy > this->numOfArmy - 1 ? this->numOfArmy - 1 : numOfArmy;
}

void Country::addNumOfArmy(int numOfArmy) {
    this->numOfArmy += numOfArmy;
}
