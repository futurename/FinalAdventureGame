#include <map>
#include "HelperFunctions/Game.h"

const vector<string> Player::DEFAULT_PLAYER_NAMES{vector<string>{"You", "Napoleon", "Trump", "Putin", "Modi"}};
int cardExchangeTime = 1;

Player::Player(int index, string name) {
    continentBonus = 0;
    cardExchangeTime = 0;
    playerIndex = index;
    if (name.empty()) {
        this->playerName = DEFAULT_PLAYER_NAMES.at(index);
    }
}

void Player::SetPlayerName(string inPlayerName) {
    playerName = inPlayerName;
}

string Player::getPlayerName() {
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

void Player::setBgColor(tuple<int, int, int, int> color) {
    this->bgColour = color;
}

void Player::setTextColor(tuple<int, int, int, int> color) {
    this->textColour = color;
}

tuple<int, int, int, int> Player::getBgColor() {
    return bgColour;
}

tuple<int, int, int, int> Player::getTextColor() {
    return textColour;
}


int Player::getNumOfCapturedCountries() {
    return capturedCountries.size();
}

int Player::getPlayerIndex() {
    return playerIndex;
}


int Player::getUndeployArmyNumber() {
    return undeployArmyNumber;
}

void Player::setUndeployArmyNumber(int newUndeployArmyNumber) {
    undeployArmyNumber = newUndeployArmyNumber;
}

/*
 * check cards first: if player can / must exchange cards
   if can exchange: do multiplication.
   return undeployed army number of the player
*/
void Player::getCalUndeployArmyNumber() {



    //FIXME
    this->undeployArmyNumber = 3+ continentBonus + Card::exchangeCards(cards);

}

void Player::removeUndeployArmy(int numOfArmy) {
    undeployArmyNumber -= numOfArmy;
}
