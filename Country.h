
#include "HelperFunctions/imports.h"

class Country {
public:
    Country();

    Country(string name, int x, int y, vector<string> &adjacentCountries);

    Country(string name, int x, int y, vector<string> &countries, int numOfArmy);

    string getCountryName();

    string getContinentName();

    tuple<int, int, int, int> getCountryColour();

    int getX();

    int getY();

    vector<string> &getAdjacentCountires();

    void setCountryName(string inCountryName);

    void setContinentName(string inContinentName);

    void setCountryColour(tuple<int, int, int, int> inCountryColour);

    void setCountryOwner(string inOwnerName);

    int getCountryArmy();

    tuple<int,int,int,int> getTextColor();

    void loseOneArmy();

    int getOwnerIndex();

    void setOwnerIndex(int index);

    void setTextColor(tuple<int,int,int,int> color);

    int getNumOfArmy();

    void setNumOfArmy(int newNumOfArmy);


private:
    string countryName;
    string continentName;
    tuple<int, int, int, int> countryColour;
    tuple<int,int,int,int> textColor;
    int ownerIndex;
    int coordinateX;
    int coordinateY;
    vector<string> adjacentCountries;
    int numOfArmy;
    const static int NO_COUNTRY_OWNER = -1;
};