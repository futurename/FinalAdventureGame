#include "Game.h"
#include "../Country.h"

vector<Country> Game::allCountries{vector<Country>()};

vector<Player> Game::getPlayers() {
    return vector<Player>();
}

void Game::startGame() {

}

void Game::setAllCountries(vector<Country>& countries) {
    Game::allCountries = countries;
}

void Game::printAllCountries() {
    for(Country c: allCountries){
        cout << c.getCountryName() << ", " << c.getX() << " : " << c.getY() << " ==> ";
        for(string str: c.getAdjacentCountires()){
            cout << str << ", ";
        }
        cout << endl;
    }
}

vector<Country> &Game::getAllCountries() {
    return allCountries;
}

