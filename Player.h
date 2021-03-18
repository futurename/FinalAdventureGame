#include "HelperFunctions//imports.h"
#include "HelperFunctions/Card.h"

class Country;

class Player {
public:
    Player(int index, string name = "");

    // Accessors and mutators for private attributes
    void SetPlayerName(string inPlayerName);

    string getPlayerName();

    void getPlayerCaptures();

    void addCard(CardType card);

    void setCards(vector<CardType>& cards);

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