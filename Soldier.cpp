#include "Soldier.h"


Soldier::Soldier(string sName, string cName, string c, string o){
    soldierName = sName;
    continentName = cName;
    color = c;
    owner = o;
}

void Soldier::setSoldierName(string newSoldierName){
    soldierName = newSoldierName;
}

void Soldier::setContinentName(string newContinentName){
    continentName = newContinentName;
}

void Soldier::setColor(string newColor){
    color = newColor;
}

void Soldier::setOwner(string newOwner){
    owner = newOwner;
}

void Soldier::setAttackValue(int newAttackValue){
    attackValue = newAttackValue;
}

void Soldier::setRange(int newRange){
    range = newRange;
}

void Soldier::setLevel(int newLevel){
    level = newLevel;
}

void Soldier::setExperience(int newExperience){
    experience = newExperience;
}



string Soldier::getSoliderName(){
    return soldierName;
}

string Soldier::getContinentName(){
    return continentName;
}

string Soldier::getColor(){
    return color;
}

string Soldier::getOwner(){
    return owner;
}

int Soldier::getAttackValue(){
    return attackValue;
}

int Soldier::getRange(){
    return range;
}

int Soldier::getLevel(){
    return level;
}

int Soldier::getExperience(){
    return experience;
}