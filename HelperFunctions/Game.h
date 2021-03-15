#include "imports.h"
#include "../Player.h"

class Game {
private:
    static vector<Player> players;
    static vector<Country> allCountries;

public:
    static vector<Player> getPlayers();

    static void startGame();

    static vector<Country>& getAllCountries();

    static void setAllCountries(vector<Country>& countries);

    static void printAllCountries() ;
};
