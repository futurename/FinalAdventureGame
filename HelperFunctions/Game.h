#include "imports.h"
#include "../Player.h"

class Game {
private:
    static vector<Player> players;
    static map<string, Country> allCountries;

public:
    static vector<Player> getPlayers();

    static void startGame();

    static map<string,Country>& getAllCountries();

    static void setAllCountries(map<string,Country>& countries);

    static void printAllCountries() ;
};
