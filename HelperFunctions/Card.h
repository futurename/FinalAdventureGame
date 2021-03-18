#include "imports.h"
enum CardType {
    ARTILLERY, CAVALRY, INFANTRY
};

class Card {
private:
    static const int CARD_EXCHANGE_BASE = 5;
    static const int CARD_EXCHANGE_THRESHOLD = 3;
public:
    static int exchangeCards(vector<CardType> &cards, int exchangeTimes);
};
