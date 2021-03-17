#include "Game.h"
#include "../Random.h"
#include "MapManager.h"
#include "ColorList.h"
#include <algorithm>

map<string, Country> Game::allCountries{map<string, Country>()};

map<string, Continent> Game::allContinents{map<string, Continent>()};

vector<Player> Game::players{vector<Player>()};

int Game::curPlayerIndex = 0;

const int Game::DEFAULT_NUM_UNDEPLOYED = 3;

GameStage Game::curGameStage{EXCHANGE_CARDS};

vector<Player> Game::getAllPlayers() {
    return players;
}

static MapManager mapManager;

void Game::initPlayersAndCountries() {
    //init all players
    initPlayers();

    //load map file and world list
    //MapManager::start();

    //MapManager::readMapConfigFromFile();

    //randomly assign all countries to the players evenly
    assignCountriesToPlayers();

    //check whether a continent is owned by a player
    checkInitContinentsOwner();

    //init current player to 0
    curPlayerIndex = 0;
}

void Game::initPlayers() {
    for (int i = 0; i < DEFAULT_PLAYERS; i++) {
        Player player(i);
        pair<tuple<int, int, int, int>, tuple<int, int, int, int>> color = ColorList::getPlayerColorPairs().at(i);
        player.setBgColor(color.first);
        player.setTextColor(color.second);
        players.push_back(player);
    }
}

void Game::setAllCountries(map<string, Country> &countries) {
    Game::allCountries = countries;
}

void Game::printAllCountries() {
    for (auto &country : allCountries) {
        cout << "<" << country.second.getOwnerIndex() << ">: " <<
             country.second.getCountryName() << ", " << country.second.getX() << " : " << country.second.getY()
             << " ("
             << country.second.getContinentName() << ") ==> ";
        for (const string &str: country.second.getAdjacentCountires()) {
            cout << str << ", ";
        }
        cout << endl;
    }
}

map<string, Country> &Game::getAllCountries() {
    return allCountries;
}

void Game::setAllContinents(map<string, Continent> &continents) {
    allContinents = continents;
}

map<string, Continent> &Game::getAllContinents() {
    return allContinents;
}

void Game::printAllContinents() {
    for (auto itr = allContinents.begin(); itr != allContinents.end(); itr++) {
        cout << "<" << itr->second.getOwnerIndex() << ">: "
             << itr->second.getContinentName() << " : " << itr->second.getBonus() << " ==> ";
        for (string name : itr->second.getCountryNames()) {
            cout << name << ", ";
        }
        cout << endl;
    }
}

//Check whether all countries of a continent are conquered by the player. If so, the player is given an amount of
//armies that corresponding to the continent's control value.
void Game::attackFrom(Country attacker, Country defender) {
    for (int i = 0; i < attacker.getCountryArmy(); i++) {
        pair<int, int> attackerDice = Random::RollDie(attacker.getCountryArmy());
        pair<int, int> defenderDice = Random::RollDie(defender.getCountryArmy());

        if ((attackerDice.first > defenderDice.first)
            || (attackerDice.first == defenderDice.first && attackerDice.second > defenderDice.second)) {
            defender.loseOneArmy();
        } else if (attackerDice.first == defenderDice.first && attackerDice.second == defenderDice.second) {
            continue;
        } else {
            attacker.loseOneArmy();
        }

        if (attacker.getCountryArmy() == 1) {
            break;
        }

        if (defender.getCountryArmy() == 0) {
            conquerTheCountry(attacker, defender);
        }

        Continent &continent = allContinents.at(defender.getContinentName());

        if (isContinentConquered(attacker.getOwnerIndex(), continent.getContinentName())) {
            continent.setOwnerIndex(attacker.getOwnerIndex());
            players.at(attacker.getOwnerIndex())
                    .addContinentBonus(continent.getBonus());
        }
    }
}

bool Game::isContinentConquered(int index, const string &continentName) {
    vector<string> &countryNames = allContinents.find(continentName)->second.getCountryNames();
    bool result = true;
    for (const string &countryName: countryNames) {
        if (allCountries.find(countryName)->second.getOwnerIndex() != index) {
            result = false;
            break;
        }
    }
    return result;
}

void Game::assignCountriesToPlayers() {
    int avgCountryNumber = allCountries.size() / players.size();
    vector<int> playerCounter(players.size(), 0);

    for (int i = 0; i < players.size(); i++) {
        playerCounter.at(i) = avgCountryNumber;
    }

    //remaining countries will be assigned to prior players
    for (int i = 0; i < allCountries.size() - avgCountryNumber * players.size(); i++) {
        playerCounter.at(i)++;
    }

    for (auto &country : allCountries) {
        int rand;
        while (true) {
            rand = Random::GenerateRandomNum(0, players.size() - 1);
            if (playerCounter.at(rand) > 0) {
                break;
            }
        }
        playerCounter.at(rand)--;
        country.second.setOwnerIndex(rand);
        country.second.setCountryColour(players.at(rand).getBgColor());
        country.second.setTextColor(players.at(rand).getTextColor());
    }
}

void Game::checkInitContinentsOwner() {
    for (auto &continent: allContinents) {
        for (int i = 0; i < players.size(); i++) {
            if (isContinentConquered(i, continent.first)) {
                players.at(i).addContinentBonus(continent.second.getBonus());
            }
        }
    }
}


/*
 *0. validate the dragged number is equal to or less than the remaining number.
  1. init undeployed army numbers: 3 by default + bonus from the player attr.
  2. after the player selects a country and a certain number, add the army num to the country and subtract this from the total undeployed army number.
  3. the function is called repeatedly until there is no more undeployed army.
*/
bool Game::deployArmy(Country& country, Player& player, int numToDeploy) {

    //Num of armies given depends on player's captured countries. Min is 3.
    int numOfArmyGiven = max(DEFAULT_NUM_UNDEPLOYED, player.getNumOfCapturedCountries() / 3);

    //inital undeployed armies is player's undeployed army number + num of armies given
    int totalUndeployed = player.getCalUndeployArmyNumber() + numOfArmyGiven;

    //bonus for conquering continent(s)
    for (const auto &item: allContinents) {
        if (item.second.getOwnerIndex() != player.getPlayerIndex()) {
            totalUndeployed += item.second.getBonus();
        }
    }

    //add player's chosen army number to the country
    int newNumOfArmy = country.getNumOfArmy() + numToDeploy;;
    if (numToDeploy <= totalUndeployed)

        country.setNumOfArmy(newNumOfArmy);

        //subtract from total undeployed army number and update player's undeployed
        totalUndeployed ;

        player.setUndeployArmyNumber(totalUndeployed - newNumOfArmy);
    }



//    bool RemainingIsZero = totalUndeployed < 0;
//
//    if (totalUndeployed <= 0){
//        RemainingIsZero = true;
//    }


    return (totalUndeployed <= 0);



}

void Game::conquerTheCountry(Country &attackCountry, Country &defendCountry) {
    Player &attacker = players.at(attackCountry.getOwnerIndex());
    Player &defender = players.at(defendCountry.getOwnerIndex());

    stringstream ss;

    defendCountry.setOwnerIndex(attacker.getPlayerIndex());
    defendCountry.setTextColor(attacker.getTextColor());
    defendCountry.setCountryColour(attacker.getBgColor());

    Continent &continent = allContinents.at(defendCountry.getContinentName());

    if (continent.getOwnerIndex() == defender.getPlayerIndex()) {
        continent.setOwnerIndex(Continent::NO_CONTINENT_OWNER);
        defender.removeContinentBonus(continent.getBonus());
    }

    if (isContinentConquered(attackCountry.getOwnerIndex(), continent.getContinentName())) {
        ss << "Conquer " << continent.getContinentName() << "!";
        string title = ss.str();
        ss.str("");
        ss << attacker.getPlayerName() << " gets bonus <" << continent.getBonus() << ">";
        string body = ss.str();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title.c_str(), body.c_str(), NULL);
        continent.setOwnerIndex(attackCountry.getOwnerIndex());
        attacker.addContinentBonus(continent.getBonus());
    }


    // func: bool gameIsOver()
    bool gameOver = true;
    for (auto &c: allContinents) {
        if (c.second.getOwnerIndex() != attacker.getPlayerIndex()) {
            gameOver = false;
            break;
        }
    }

    if (gameOver) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_COLOR_TEXT, "GAME OVER", (attacker.getPlayerName() + " WIN!").c_str(),
                                 NULL);

        // break or reset
    }
}

void Game::setCurPlayerIndex(int index) {
    curPlayerIndex = index;
}

int Game::getCurPlayerIndex() {
    return curPlayerIndex;
}

void Game::setGameStage(GameStage stage) {
    curGameStage = stage;
}

GameStage Game::getGameStage() {
    return curGameStage;
}


void Game::runGame(){

    //call after deploy
    MapManager::updateWholeScreen();
}


