#include "imports.h"
#include "../Player.h"
#include "../Continent.h"

class Game {
private:
    static vector<Player> players;
    static map<string, Country> allCountries;      //<countryName, countryObject>
    static map<string, Continent> allContinents;   //<continentName, continentObject>
    const static int DEFAULT_PLAYERS = 3;
    const static int DEFAULT_NUM_UNDEPLOYED = 3;   //defaul num of undeployed armies

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

    static int deployArmy(Country& country, Player player, int numOfDeployed);
};
