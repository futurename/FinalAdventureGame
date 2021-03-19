#include "Card.h"
#include "Random.h"

int Card::exchangeCards(vector<CardType> &cards, int exchangeTimes) {
    int result = 0;

    if (cards.size() <= 2) {
        return result;
    }

    sort(cards.begin(), cards.end());

    CardType preCardType = cards.at(cards.size() - 1);
    int sameTypeCounter = 1;
    int diffTypeCounter = 1;

    for (int i = cards.size() - 2; i >= 0; i--) {
        CardType curCardType = cards.at(i);
        if (curCardType == preCardType) {
            sameTypeCounter++;
        } else {
            preCardType = curCardType;
            sameTypeCounter = 1;
            diffTypeCounter++;
        }
        if (sameTypeCounter == CARD_EXCHANGE_THRESHOLD) {
            cards.erase(cards.begin() + i, cards.begin() + i + sameTypeCounter);
            result += CARD_EXCHANGE_BASE * exchangeTimes;
            break;
        }

        if (diffTypeCounter == CARD_EXCHANGE_THRESHOLD) {
            cards.erase(cards.end() - 1);
            cards.erase(cards.begin() + i);
            for (int j = cards.size() - 2; j >= 0; j--) {
                if (cards.at(j) == CAVALRY) {
                    cards.erase(cards.begin() + j);
                    break;
                }
            }
            result += CARD_EXCHANGE_BASE * exchangeTimes;
            break;
        }
    }
    return result;
}

CardType Card::getBonusCard() {
    int randIndex = Random::generateRandomNum(0,2);
    switch (randIndex) {
        case 0:
            return ARTILLERY;
        case 1:
            return CAVALRY;
        default:
            return INFANTRY;
    }
}
