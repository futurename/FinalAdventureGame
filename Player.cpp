#include "Player.h"
#include "Country.h"

Player::Player(int index) {
    playerName = "P_" + to_string(index + 1);
    continentBonus = 0;
    cardExchangeTime = 0;
    playerIndex = index;
}


void Player::SetPlayerName(string inPlayerName) {
    playerName = inPlayerName;
}

string Player::GetPlayerName() {
    return playerName;
}

void Player::addCard(Card card) {
    cards.push_back(card);
}

void Player::setCards(vector<Card> newCards) {
    cards = newCards;
}

vector<Card> Player::getCards() {
    return cards;
}

void Player::addContinentBonus(int bonus) {
    continentBonus += bonus;
}

void Player::removeContinentBonus(int bonus) {
    continentBonus -= bonus;
}

