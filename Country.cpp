#include "Country.h"
#include "StaticFunc/ColorList.h"

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

string Country::getOwnerName() {
    return ownerName;
}

void Country::setCountryOwner(string inOwnerName) {
    ownerName = inOwnerName;
}

Country::Country(string name, int x, int y, vector<string> &countries) {
    this->countryName = name;
    this->coordinateX = x;
    this->coordinateY = y;
    this->countryColour = ColorList::GREEN;
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
