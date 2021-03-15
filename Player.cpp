#include "Player.h"
#include "Country.h"

Player::Player(){
    playerName = "player";
}


void Player::SetPlayerName(string inPlayerName){
    playerName = inPlayerName;
}
string Player::GetPlayerName(){
    return playerName;
}

void Player::addCard(Card card){
    cards.push_back(card);
}

void Player::setCards(vector<Card> newCards){
    cards = newCards;
}

vector<Card> Player::getCards(){
    return cards;
}

