#ifndef FINALGAMEDESIGN_GAME_H
#define FINALGAMEDESIGN_GAME_H
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include "imports.h"
#include "../Player.h"
#include "../Continent.h"
#include "Card.h"

enum GameStage {
    DEPLOYMENT, ATTACK, MOVE, EXCHANGE_CARDS
};

class Game {
private:
    static vector<Player> players;
    static map<string, Country> allCountries;      //<countryName, countryObject>
    static map<string, Continent> allContinents;   //<continentName, continentObject>
    const static int DEFAULT_PLAYERS = 4;

    static int curPlayerIndex;
    static GameStage curGameStage;
    const static string DEFAULT_MAP_CONFIG;
    const static int ROBOT_MAX_RAND_NUM_UPPER = 6;
    const static int ROBOT_MAX_RAND_NUM_LOWER = 2;
public:
    const static int DEFAULT_NUM_UNDEPLOY;
    static bool isConquerACountry;
    static bool ifClickedNext;
    static bool isHumanPlayer;
    const static string DEFAULT_MAP;

    static vector<Player> &getAllPlayers();

    static void initPlayersAndCountries();

    static map<string, Country> &getAllCountries();

    static void setAllCountries(map<string, Country> &countries);

    static void setAllContinents(map<string, Continent> &continents);

    static map<string, Continent> &getAllContinents();

    static void printAllCountries();

    static void printAllContinents();

    static void attackFrom(Country& attackCountry, Country& defendCountry);

    static bool isContinentConquered(int index, const string &continentName);

    static void initPlayers();

    static void assignCountriesToPlayers();

    static void checkInitContinentsOwner();

    static bool deployArmy(Country &country, Player &player, int numOfDeployed);

    static void conquerTheCountry(Country &attackCountry, Country &defendCountry);

    static void setCurPlayerIndex(int index);

    static int getCurPlayerIndex();

    static void setGameStage(GameStage stage);

    static GameStage getGameStage();

    static void runGame();

    static void readMapConfigFromFile(string filePath = DEFAULT_MAP_CONFIG);

    static void robotDeploy();

    static void robotAttack();

    static void robotMove();

    static vector<Country*> getPlayerCountries(int playerIndex);

    static bool hasAdjEnemyCountry(Country *pCountry);

    static void saveMapConfigFromFile(string filePath = DEFAULT_MAP_CONFIG);    // Save

    static void loadGameConfigFromFile(string filePath = DEFAULT_MAP_CONFIG);   // load
};
#endif
