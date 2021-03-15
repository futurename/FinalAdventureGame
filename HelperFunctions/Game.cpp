#include "Game.h"
#include "../Country.h"

map<string, Country> Game::allCountries{map<string, Country>()};

vector<Player> Game::getPlayers() {
    return vector<Player>();
}

void Game::startGame() {

}

void Game::setAllCountries(map<string,Country> &countries) {
    Game::allCountries = countries;
}

void Game::printAllCountries() {
    for (auto itr = allCountries.begin(); itr!= allCountries.end(); itr++) {
        cout << itr->second.getCountryName() << ", " << itr->second.getX() << " : " << itr->second.getY() << " ==> ";
        for (string str: itr->second.getAdjacentCountires()) {
            cout << str << ", ";
        }
        cout << endl;
    }
}

map<string, Country> &Game::getAllCountries() {
    return allCountries;
}

