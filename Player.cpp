#include "Player.h"
#include "Country.h"

const vector<string> Player::DEFAULT_PLAYER_NAMES{vector<string>{"You", "Putin", "Trump", "Modi","Xi"}};

Player::Player(int index, string name) {
    continentBonus = 0;
    cardExchangeTime = 0;
    playerIndex = index;
    if(name.empty()){
        this->playerName = DEFAULT_PLAYER_NAMES.at(index);
    }
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

