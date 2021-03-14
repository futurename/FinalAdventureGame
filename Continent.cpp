#include "Continent.h"
#include "Country.h"

Continent::Continent(string cName, int cBonusValue){
    continentName = cName;
    continentOwnerIndex = -1;
    continentBonusValue = cBonusValue;
    map<string, Country> continentCountryGraph;
}


string Continent::getContinentName(){
    return continentName;
}

int Continent::getContinentOwnerIndex(){
    return continentOwnerIndex;
}

int Continent::getContinentBonusValue(){
    return continentBonusValue;
}

map<string, Country> Continent::getContinentCountryGraph(){
    return continentCountryGraph;
}

