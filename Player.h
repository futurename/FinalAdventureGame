#include "HelperFunctions//imports.h"
#include "Card.h"

class Country;

class Player {
public:
    Player(int index);

    // Accessors and mutators for private attributes
    void SetPlayerName(string inPlayerName);

    string GetPlayerName();

    void GetPlayerCaptures();

    void addCard(Card card);

    void setCards(vector<Card> cards);

    vector<Card> getCards();

    void addContinentBonus(int bonus);

    void removeContinentBonus(int bonus);

private:
    string playerName;
    vector<Card> cards;
    vector<Country> capturedCountries;
    tuple<int, int, int, int> bgColour;
    tuple<int, int, int, int> textColour;
    int continentBonus;
    int cardExchangeTime;
    int playerIndex;
};