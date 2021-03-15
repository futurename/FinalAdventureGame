#pragma once

#include "HelperFunctions/imports.h"
#include "Country.h"

class Continent {
private:
    string continentName;
    int ownerIndex;
    int bonus;
    vector<string> countryNames;

public:
    Continent(string cName, int cBonusValue);

    string getContinentName();

    int getOwnerIndex();

    int getBonus();

    void addCountryName(string &countryName);

    vector<string> &getCountryNames();

    void setCountryNames(vector<string>& countryNames);
};