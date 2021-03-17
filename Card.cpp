
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
