#pragma once
#include "imports.h"

class Country {
public:
    string GetCountryName();
    string GetContinentName();
    string GetCountryColour();
    string GetOwnerName();

    void SetCountryName(string inCountryName);
    void SetContinentName(string inContinentName);
    void SetCountryColour(string inCountryColour);
    void SetCountryOwner(string inOwnerName);

private:
    string countryName;
    string continentName;
    string countryColour;
    string ownerName;
};