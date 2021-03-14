#pragma once
#include "StaticFunc/imports.h"
#include "Country.h"

class Continent{
private:
    string continentName;
    int continentOwnerIndex;
    int continentBonusValue;
    map<string, Country> continentCountryGraph;

public:
    Continent();
    Continent(string cName, int cBonusValue);
    string getContinentName();
    int getContinentOwnerIndex();
    int getContinentBonusValue();
    map<string, Country> getContinentCountryGraph();



};