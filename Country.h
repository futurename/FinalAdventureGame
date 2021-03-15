
#include "HelperFunctions/imports.h"


class Country {
public:
    Country();

    Country(string name, int x, int y, vector<string> &adjacentCountries);

    string getCountryName();

    string getContinentName();

    tuple<int, int, int, int> getCountryColour();

    string getOwnerName();

    int getX();

    int getY();

    vector<string> &getAdjacentCountires();

    void setCountryName(string inCountryName);

    void setContinentName(string inContinentName);

    void setCountryColour(tuple<int, int, int, int> inCountryColour);

    void setCountryOwner(string inOwnerName);

    int getCountryArmy();

private:
    string countryName;
    string continentName;
    tuple<int, int, int, int> countryColour;
    string ownerName;
    int coordinateX;
    int coordinateY;
    vector<string> adjacentCountries;
    int numOfArmy;
};