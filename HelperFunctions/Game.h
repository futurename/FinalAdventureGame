#include "imports.h"
#include "../Player.h"
#include "../Continent.h"

enum GameState{DEPLOYMENT, ATTACK, MOVE, EXCHANGE_CARDS};

class Game {
private:
    static vector<Player> players;
    static map<string, Country> allCountries;
    static map<string, Continent> allContinents;
    const static int DEFAULT_PLAYERS = 4;

public:
    static vector<Player> getPlayers();

    static void initPlayersAndCountries();

    static map<string, Country> &getAllCountries();

    static void setAllCountries(map<string, Country> &countries);

    static void setAllContinents(map<string, Continent> &continents);

    static map<string, Continent> &getAllContinents();

    static void printAllCountries();

    static void printAllContinents();

    static void attackFrom(Country attacker, Country defender);

    static bool isContinentConquered(int index, const string& continentName);

    static void initPlayers();

    static void assignCountriesToPlayers();

    static void checkInitContinentsOwner();

    static void conquerTheCountry(Country &attackCountry, Country &defendCountry);
};
