#include "Continent.h"

Continent::Continent(string cName, int cBonusValue){
    continentName = cName;
    ownerIndex = -1;
    bonus = cBonusValue;

}


string Continent::getContinentName(){
    return continentName;
}

int Continent::getOwnerIndex(){
    return ownerIndex;
}

int Continent::getBonus(){
    return bonus;
}

void Continent::addCountryName(string& countryName) {
    countryNames.push_back(countryName);
}

vector<string>& Continent::getCountryNames() {
    return countryNames;
}

void Continent::setCountryNames(vector<string>& countryNames) {
    this->countryNames = countryNames;
}


