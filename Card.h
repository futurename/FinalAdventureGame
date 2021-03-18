#include "HelperFunctions/imports.h"

enum CardType {
    ARTILLERY, CAVALRY, INFANTRY
};

class Card {

private:
    CardType cardType;
    static const int CARD_EXCHANGE_BASE = 5;
    static const int CARD_EXCHANGE_THRESHOLD = 3;
public:

    Card(CardType card);

    CardType getCard();

    void setCard(CardType newCardType);

    static int exchangeCards(vector<CardType> &cards, int exchangeTimes);
};
