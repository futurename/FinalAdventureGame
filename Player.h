#include "StaticFunc/imports.h"

class Country;

class Player {
public:
    Player();

    // Accessors and mutators for private attributes
    void SetPlayerName(string inPlayerName);

    string GetPlayerName();

    void GetPlayerCaptures();

private:
    string playerName;
    vector<Country> capturedCountries;
    tuple<int,int,int,int> bgColour;
    tuple<int,int,int,int> textColour;


};