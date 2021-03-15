#include "Game.h"

map<string, Country> Game::allCountries{map<string, Country>()};

map<string, Continent> Game::allContinents{map<string, Continent>()};

vector<Player> Game::getPlayers() {
    return vector<Player>();
}

void Game::startGame() {

}

void Game::setAllCountries(map<string, Country> &countries) {
    Game::allCountries = countries;
}

void Game::printAllCountries() {
    for (auto itr = allCountries.begin(); itr != allCountries.end(); itr++) {
        cout << itr->second.getCountryName() << ", " << itr->second.getX() << " : " << itr->second.getY() << " ("
             << itr->second.getContinentName() << ") ==> ";
        for (string str: itr->second.getAdjacentCountires()) {
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
        cout << itr->second.getContinentName() << " : " << itr->second.getBonus() << " ==> ";
        for (string name : itr->second.getCountryNames()) {
            cout << name << ", ";
        }
        cout  << endl;
    }
}

void Game::attackFrom(Country attacker, int army, Country defender) {
    while(true){

    }
}

