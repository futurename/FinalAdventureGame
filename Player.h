#ifndef FINALGAMEDESIGN_PLAYER_H
#define FINALGAMEDESIGN_PLAYER_H

#include "HelperFunctions//imports.h"
#include "HelperFunctions/Card.h"

class Country;

class Player {
public:
    Player(int index, string name = "");

    string getPlayerName();

    vector<Country> getCapturedCountries();

    vector<string> getCapturedCountryNames();

    void addCard(CardType card);

    vector<CardType>& getCards();

    void addContinentBonus(int bonus);

    void removeContinentBonus(int bonus);

    void setBgColor(tuple<int,int,int,int> color);

    void setTextColor(tuple<int,int,int,int> color);

    tuple<int,int,int,int> getBgColor();

    tuple<int,int,int,int> getTextColor();

    int getNumOfCapturedCountries();

    int getPlayerIndex() const;

    int getUndeployArmyNumber() const;

    void setUndeployArmyNumber(int newUndeployArmyNumber);

    void getCalUndeployArmyNumber();

    void removeUndeployArmy(int numOfArmy);

    int getCardNumOfType(CardType type);

    int getContinentBonus();

    int getExchangeTimes();

    void setExchangeTimes(int newExchangeTimes);

    void setCapturedCountries(vector<Country>& newCapturedCountries);


private:
    string playerName;
    vector<CardType> cards;
    vector<Country> capturedCountries;
    tuple<int, int, int, int> bgColour;
    tuple<int, int, int, int> textColour;
    int continentBonus;
    int exchangeTimes = 1;
    int playerIndex;
    const static vector<string> DEFAULT_PLAYER_NAMES;
    int undeployArmyNumber = 0;
};
#endif