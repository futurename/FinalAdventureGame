
#include "Card.h"

Card::Card(CardType cardType){
    this->cardType = cardType;
}

CardType Card::getCard(){
    return cardType;
}

void Card::setCard(CardType newCardType){
    cardType = newCardType;
}

int Card::exchangeCards(vector<Card> cards) {
    // a map to store the number of cards of each type
    map<CardType, int> cardMap;
    cardMap[INFANTRY] = 0;
    cardMap[CAVALRY] = 0;
    cardMap[ARTILLERY] = 0;

    for (size_t i = 0; i < cards.size(); i++) {
        //get the card types from the player's cards
        cardMap[cards.at(i).getCard()] += 1;
    }

    bool hasThreeDifferentTypes = true;

    for (auto it = cardMap.begin(); it != cardMap.end(); it++) {
        // if having 3 same type of cards, must exchange for army
        if (it->second >= 3) {
            cardMap[it->first] -= 3;
            undeployArmyNumber += cardExchangeTime * Card::CARD_EXCHANGE_BASE;
            cardExchangeTime++;
            //also remove from player's vector of cards
        }

        if (it->second == 0) {
            hasThreeDifferentTypes = false;
        }
    }

    //exchange for army with three different types of cards
    if (hasThreeDifferentTypes) {
        undeployArmyNumber += cardExchangeTime * Card::CARD_EXCHANGE_BASE;
        cardExchangeTime++;
        for (auto it = cardMap.begin(); it != cardMap.end(); it++) {
            cardMap[it->first] -= 1;
            if (it->second == 0) {
                hasThreeDifferentTypes = false;
            }
        }
    }

    //possible to still have three different types of cards
    if (hasThreeDifferentTypes) {
        undeployArmyNumber += cardExchangeTime * Card::CARD_EXCHANGE_BASE;
        for (auto it = cardMap.begin(); it != cardMap.end(); it++) {
            cardMap[it->first] -= 1;
        }
    }

    //create new set of cards for the player then re-assign it.
    vector<Card> newCards;

    for (auto it = cardMap.begin(); it != cardMap.end(); it++) {
        if (it->second > 0) {
            for (size_t i = 0; i < it->second; i++) {
                Card newCard{it->first};
                newCards.push_back(newCard);
            }
        }
    }

    cards = newCards;
    return 0;
}
