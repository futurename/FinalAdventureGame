#include "HelperFunctions/imports.h"

enum CardType {
    INFANTRY, CAVALRY, ARTILLERY
};

class Card {

private:
    CardType cardType;
    const static int CARDS_EXCHANGE_VALUE = 5;
public:

    Card(CardType card);

    CardType getCard();

    void setCard(CardType newCardType);

    static const int CARD_EXCHANGE_BASE = 5;

    static int exchangeCards(vector<Card> cards);
};
