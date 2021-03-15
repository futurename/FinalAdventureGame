#include "HelperFunctions//imports.h"
#include "Card.h"

class Country;

class Player {
public:
    Player();

    // Accessors and mutators for private attributes
    void SetPlayerName(string inPlayerName);

    string GetPlayerName();

    void GetPlayerCaptures();

    void addCard(Card card);

    void setCards(vector<Card> cards);

    vector<Card> getCards();

private:
    string playerName;
    vector<Card> cards;
    vector<Country> capturedCountries;
    tuple<int, int, int, int> bgColour;
    tuple<int, int, int, int> textColour;

};