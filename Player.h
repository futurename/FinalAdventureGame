#pragma once
#include "StaticFunc/imports.h"
#include "Country.h"

class Player{
public:
    Player();



    // Accessors and mutators for private attributes
    void SetPlayerName(string inPlayerName);
    string GetPlayerName();

    void GetPlayerCaptures();

private:
    string playerName;
    vector<Country> capturedCountries;


};