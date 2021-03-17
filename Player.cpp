#include "Player.h"
#include "Country.h"
#include <map>
#include "Card.h"
using namespace std;

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


int Player::getNumOfCapturedCountries(){
    return capturedCountries.size();
}

int Player::getPlayerIndex() {
    return playerIndex;
}

int Player::getUndeployArmyNumber(){
    return undeployArmyNumber;
}

void Player::setUndeployArmyNumber(int newUndeployArmyNumber){
    undeployArmyNumber = newUndeployArmyNumber;
}

/*
 * check cards first: if player can / must exchange cards
   if can exchange: do multiplication.
   return undeployed army number of the player
*/
int Player::getCalUndeployArmyNumber(){

    // a map to store the number of cards of each type
    map<CardType, int> cardMap;
    cardMap[INFANTRY] = 0;
    cardMap[CALVARY] = 0;
    cardMap[ARTILLERY] = 0;

    for (size_t i = 0; i < cards.size(); i++){
        //get the card types from the player's cards
        cardMap[cards.at(i).getCard()] += 1;
    }

    bool hasThreeDifferentTypes = true;

    for (auto it = cardMap.begin(); it != cardMap.end(); it++){
        // if having 3 same type of cards, must exchange for army
        if (it->second >= 3){
            cardMap[it->first] -=3;
            undeployArmyNumber += cardExchangeTime * Card::CARD_EXCHANGE_BASE;
            cardExchangeTime++;
            //also remove from player's vector of cards
        }

        if (it->second == 0){
            hasThreeDifferentTypes = false;
        }
    }

    //exchange for army with three different types of cards
    if (hasThreeDifferentTypes){
        undeployArmyNumber += cardExchangeTime * Card::CARD_EXCHANGE_BASE;
        cardExchangeTime++;
        for (auto it = cardMap.begin(); it != cardMap.end(); it++){
            cardMap[it->first] -= 1;
            if (it->second == 0){
                hasThreeDifferentTypes = false;
            }
        }
    }

    //possible to still have three different types of cards
    if (hasThreeDifferentTypes){
        undeployArmyNumber += cardExchangeTime * Card::CARD_EXCHANGE_BASE;
        for (auto it = cardMap.begin(); it != cardMap.end(); it++) {
            cardMap[it->first] -= 1;
        }
    }

    //create new set of cards for the player then re-assign it.
    vector<Card> newCards;

    for (auto it = cardMap.begin(); it != cardMap.end(); it++){
        if (it->second > 0){
            for (size_t i = 0; i < it->second; i++){
                Card newCard {it->first};
                newCards.push_back(newCard);
            }
        }
    }

    cards = newCards;

    return undeployArmyNumber;

}
