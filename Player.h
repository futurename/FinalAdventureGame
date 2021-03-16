#include "HelperFunctions//imports.h"
#include "Card.h"

class Country;

class Player {
public:
    Player(int index, string name = "");

    // Accessors and mutators for private attributes
    void SetPlayerName(string inPlayerName);

    string getPlayerName();

    void GetPlayerCaptures();

    void addCard(Card card);

    void setCards(vector<Card> cards);

    vector<Card> getCards();

    void addContinentBonus(int bonus);

    void removeContinentBonus(int bonus);

    void setBgColor(tuple<int,int,int,int> color);

    void setTextColor(tuple<int,int,int,int> color);

    tuple<int,int,int,int> getBgColor();

    tuple<int,int,int,int> getTextColor();

private:
    string playerName;
    vector<Card> cards;
    vector<Country> capturedCountries;
    tuple<int, int, int, int> bgColour;
    tuple<int, int, int, int> textColour;
    int continentBonus;
    int cardExchangeTime;
    int playerIndex;
    const static vector<string> DEFAULT_PLAYER_NAMES;
};