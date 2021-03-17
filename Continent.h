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

    int getOwnerIndex() const;

    int getBonus() const;

    void addCountryName(string &countryName);

    vector<string> &getCountryNames();

    void setCountryNames(vector<string>& countryNames);

    void setOwnerIndex(int index);

    const static int NO_CONTINENT_OWNER = -1;
};