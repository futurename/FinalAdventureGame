//
// Created by Frank on 03/15/2021.
//
#include "Card.h"


Card::Card(PlayerCard card){
    cardType = card;
}

PlayerCard Card::getCard(){
    return cardType;
}

void Card::setCard(PlayerCard newCardType){
    cardType = newCardType;
}
