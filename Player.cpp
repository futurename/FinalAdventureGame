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