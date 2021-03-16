#include "Game.h"
#include "../Random.h"
#include "MapManager.h"
#include "ColorList.h"

map<string, Country> Game::allCountries{map<string, Country>()};

map<string, Continent> Game::allContinents{map<string, Continent>()};

vector<Player> Game::players{vector<Player>()};

vector<Player> Game::getPlayers() {
    return players;
}

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

    //render country marks
    /*for (auto &item : allCountries) {
        Country &country = item.second;
        int x = country.getX();
        int y = country.getY();

        MapManager::setOwnerColorMark(x, y, country.getCountryColour());

        MapManager::renderCountryMark(x, y, country, MapManager::COUNTRY_NAME_FONT_SIZE);
    }*/


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
            defender.setOwnerIndex(attacker.getOwnerIndex());
            defender.setCountryColour(attacker.getCountryColour());
            defender.setTextColor(attacker.getTextColor());
        }

        Continent &continent = allContinents.at(defender.getContinentName());

        if (continent.getOwnerIndex() == defender.getOwnerIndex()) {
            continent.setOwnerIndex(Continent::NO_CONTINENT_OWNER);
        }

        if (isContinentConquered(attacker.getOwnerIndex(), continent.getContinentName())) {
            continent.setOwnerIndex(attacker.getOwnerIndex());
            players.at(attacker.getOwnerIndex())
                    .addContinentBonus(continent.getBonus());
        }
        //FIXME whether the world is conquered by the attacker.
    }
}

bool Game::isContinentConquered(int index, const string &continentName) {
    vector<string> &countryNames = allContinents.find(continentName)->second.getCountryNames();
    bool result = true;
    for (const string &countryName: countryNames) {
        if (allCountries.find(countryName) == allCountries.end()) {
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

