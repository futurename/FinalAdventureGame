#pragma once
#include "StaticFunc/imports.h"

class Soldier{
private:
    string soldierName;
    string continentName;
    string color;
    string owner;
    int attackValue;
    int range = 1;
    int level;
    int experience; //passed to Policy obj

public:
    //constructors
    Soldier();
    Soldier(string sName, string cName, string c, string o);

    //setters
    void setSoldierName(string newSoldierName);
    void setContinentName(string newContinentName);
    void setColor(string newColor);
    void setOwner(string newOwner);
    void setAttackValue(int newAttackValue);
    void setRange(int newRange);
    void setLevel(int newLevel);
    void setExperience(int newExperience);

    //getters
    string getSoliderName();
    string getContinentName();
    string getColor();
    string getOwner();
    int getAttackValue();
    int getRange();
    int getLevel();
    int getExperience();

};