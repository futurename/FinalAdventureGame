#include "Game.h"
#include "Random.h"
#include "MapManager.h"
#include "ColorList.h"

map<string, Country> Game::allCountries{map<string, Country>()};
map<string, Continent> Game::allContinents{map<string, Continent>()};
vector<Player> Game::players{vector<Player>()};
int Game::curPlayerIndex = 0;
const int Game::DEFAULT_NUM_UNDEPLOY = 3;

const string Game::DEFAULT_MAP = "../Maps/World.bmp";
const string Game::DEFAULT_MAP_CONFIG = "../Maps/World.map";

/*const string Game::DEFAULT_MAP = "../Maps/Atlantis.bmp";
const string Game::DEFAULT_MAP_CONFIG = "../Maps/Atlantis.map";*/

GameStage Game::curGameStage{DEPLOYMENT};
bool Game::isConquerACountry = false;
bool Game::ifClickedNext = false;
bool Game::isHumanPlayer = curPlayerIndex == 0;

vector<Player> &Game::getAllPlayers() {
    return players;
}

static MapManager mapManager;

void Game::initPlayersAndCountries() {
    //init all players
    initPlayers();

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

//check whether the army num >1
//check the defender country is adjacent of the attacking country.
//check the owner of defending country is different from the attacking country.
//call attackFrom()
//updateWorldmap();

void Game::attackFrom(Country &attackCountry, Country &defendCountry) {
    for (int i = 0; i < attackCountry.getCountryArmy(); i++) {
        pair<int, int> attackerDice = Random::RollDie(attackCountry.getCountryArmy());
        pair<int, int> defenderDice = Random::RollDie(defendCountry.getCountryArmy());

        if ((attackerDice.first > defenderDice.first)
            || (attackerDice.first == defenderDice.first && attackerDice.second > defenderDice.second)) {
            defendCountry.loseOneArmy();
        } else if (attackerDice.first == defenderDice.first && attackerDice.second == defenderDice.second) {
            continue;
        } else {
            attackCountry.loseOneArmy();
        }

        if (attackCountry.getCountryArmy() == 1) {
            break;
        }

        if (defendCountry.getCountryArmy() == 0) {
            conquerTheCountry(attackCountry, defendCountry);
            int moveArmy = attackCountry.getCountryArmy() - 1;
            attackCountry.setNumOfArmy(1);
            defendCountry.setNumOfArmy(moveArmy);

            isConquerACountry = true;
        }
    }
}

bool Game::isContinentConquered(int index, const string &continentName) {
    vector<string> &countryNames = allContinents.find(continentName)->second.getCountryNames();
    for (const string &countryName: countryNames) {
        if (allCountries.find(countryName)->second.getOwnerIndex() != index) {
            return false;
        }
    }
    return true;
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
            rand = Random::generateRandomNum(0, players.size() - 1);
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
bool Game::deployArmy(Country &country, Player &player, int numToDeploy) {
    //Num of armies given depends on player's captured countries. Min is 3.
    int numOfArmyGiven = max(DEFAULT_NUM_UNDEPLOY, player.getNumOfCapturedCountries() / 3);

    //inital undeployed armies is player's undeployed army number + num of armies given
    int totalUndeployed = player.getUndeployArmyNumber() + numOfArmyGiven;

    //bonus for conquering continent(s)
    for (const auto &item: allContinents) {
        if (item.second.getOwnerIndex() != player.getPlayerIndex()) {
            totalUndeployed += item.second.getBonus();
        }
    }

    //add player's chosen army number to the country
    int newNumOfArmy = country.getNumOfArmy() + numToDeploy;;
    if (numToDeploy <= totalUndeployed) {

        country.setNumOfArmy(newNumOfArmy);

        //subtract from total undeployed army number and update player's undeployed

        player.setUndeployArmyNumber(totalUndeployed - newNumOfArmy);
    }


    return totalUndeployed <= 0;
}

void Game::conquerTheCountry(Country &attackCountry, Country &defendCountry) {
    Player &attacker = players.at(attackCountry.getOwnerIndex());
    Player &defender = players.at(defendCountry.getOwnerIndex());

    stringstream ss;

    Continent &continent = allContinents.at(defendCountry.getContinentName());

    if (continent.getOwnerIndex() == defender.getPlayerIndex()) {
        continent.setOwnerIndex(Continent::NO_CONTINENT_OWNER);
        defender.removeContinentBonus(continent.getBonus());
    }

    defendCountry.setOwnerIndex(attacker.getPlayerIndex());
    defendCountry.setTextColor(attacker.getTextColor());
    defendCountry.setCountryColour(attacker.getBgColor());

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


void Game::runGame() {
    MapManager::start(DEFAULT_MAP);
}

void Game::readMapConfigFromFile(string filePath) {
    fstream inFIle(filePath);
    if (!inFIle.is_open()) {
        cout << "Failed reading file from the path: " << filePath << endl;
    } else {
        string line;
        map<string, Country> allCountries;
        while (getline(inFIle, line)) {
            if (line.find(MapManager::CONTINENT_TITLE) != string::npos) {
                map<string, Continent> allContinents;
                while (getline(inFIle, line)) {
                    if (line.length() == 0) {
                        break;
                    } else {
                        stringstream ss(line);
                        string continentName;
                        string bonus;

                        getline(ss, continentName, '=');
                        getline(ss, bonus, '=');

                        Continent continent(continentName, stoi(bonus));
                        allContinents.insert({continentName, continent});
                    }
                }
                setAllContinents(allContinents);
            }

            if (line.find(MapManager::TERRITORIES_TITLE) != string::npos) {
                while (getline(inFIle, line)) {
                    if (line.length() == 0) {
                        continue;
                    } else {
                        stringstream ss(line);
                        vector<string> countryTokens;

                        while (ss.good()) {
                            string subString;
                            getline(ss, subString, ',');
                            countryTokens.push_back(subString);
                        }
                        ss.str("");
                        ss.clear();

                        string countryName = countryTokens.at(MapManager::COUNTRY_NAME_INDEX);
                        int coordinateX = stoi(countryTokens.at(MapManager::COUNTRY_COORDINATE_X)) *
                                          MapManager::IMAGE_WIDTH_RATIO;
                        int coordinateY = stoi(countryTokens.at(MapManager::COUNTRY_COORDINATE_Y)) *
                                          MapManager::IMAGE_HEIGHT_RATIO;
                        string continentName = countryTokens.at(MapManager::CONTINENT_NAME_INDEX);
                        int numOfArmy = stoi(countryTokens.at(MapManager::ARMY_NUMBER_INDEX));
                        getAllContinents().find(continentName)->second.addCountryName(countryName);

                        vector<string> adjacentCountries;
                        for (int i = MapManager::ADJACENT_COUNTRIES_STARTS; i < countryTokens.size(); i++) {
                            adjacentCountries.push_back(countryTokens.at(i));
                        }
                        Country country(countryName, coordinateX, coordinateY, adjacentCountries, numOfArmy);
                        country.setContinentName(continentName);
                        allCountries.insert({countryName, country});
                    }
                }
            }
        }
        setAllCountries(allCountries);
    }
}

void Game::robotDeploy() {
    Player &robotPlayer = Game::getAllPlayers().at(Game::curPlayerIndex);
    vector<Country *> robotCountries = getPlayerCountries(curPlayerIndex);
    robotPlayer.getCalUndeployArmyNumber();
    int armyToDeploy = robotPlayer.getUndeployArmyNumber();
    Country *maxArmyCountry = nullptr;
    for (auto &country : robotCountries) {
        if (hasAdjEnemyCountry(country)) {
            maxArmyCountry = country;
            break;
        }
    }
    for (int i = 0; i < robotCountries.size(); i++) {
        if (hasAdjEnemyCountry(robotCountries.at(i)) &&
            robotCountries.at(i)->getNumOfArmy() > maxArmyCountry->getNumOfArmy()) {
            maxArmyCountry = robotCountries.at(i);
        }
    }
    maxArmyCountry->addNumOfArmy(armyToDeploy);
    robotPlayer.removeUndeployArmy(armyToDeploy);
}

void Game::robotAttack() {
    int randMax = Random::generateRandomNum(ROBOT_MAX_RAND_NUM_LOWER, ROBOT_MAX_RAND_NUM_UPPER);
    while (true) {
        Player &robotPlayer = Game::getAllPlayers().at(Game::curPlayerIndex);
        vector<Country *> robotCountries = getPlayerCountries(curPlayerIndex);

        Country *maxArmyCountry = robotCountries.at(0);
        for (int i = 1; i < robotCountries.size(); i++) {
            if (robotCountries.at(i)->getNumOfArmy() > maxArmyCountry->getNumOfArmy()) {
                if (hasAdjEnemyCountry(robotCountries.at(i))) {
                    maxArmyCountry = robotCountries.at(i);
                }
            }
        }
        vector<string> adjacentCountryNames = Game::getAllCountries().at(
                maxArmyCountry->getCountryName()).getAdjacentCountires();
        if (maxArmyCountry->getNumOfArmy() < randMax || !hasAdjEnemyCountry(maxArmyCountry)) {
            break;
        }
        Country *weakestCountry = nullptr;
        for (auto &countryName : adjacentCountryNames) {
            Country &adjCountry = Game::getAllCountries().at(countryName);
            if (adjCountry.getOwnerIndex() == curPlayerIndex) {
                continue;
            } else {
                if (weakestCountry == nullptr) {
                    weakestCountry = &adjCountry;
                } else {
                    weakestCountry =
                            weakestCountry->getNumOfArmy() >= adjCountry.getCountryArmy() ? &adjCountry
                                                                                          : weakestCountry;
                }
            }
        }

        if (weakestCountry != nullptr) {
            Game::attackFrom(*maxArmyCountry, *weakestCountry);
        }
    }
}

void Game::robotMove() {
    Player &robotPlayer = Game::getAllPlayers().at(Game::curPlayerIndex);
    vector<Country *> robotCountries = getPlayerCountries(curPlayerIndex);

    Country *maxArmyCountry = robotCountries.at(0);

    for (auto &robotCountry : robotCountries) {
        if (robotCountry->getNumOfArmy() > maxArmyCountry->getNumOfArmy()) {
            maxArmyCountry = robotCountry;
        }
    }

    if (!hasAdjEnemyCountry(maxArmyCountry)) {
        vector<string> &adjCountries = Game::getAllCountries().at(
                maxArmyCountry->getCountryName()).getAdjacentCountires();
        int randNum = Random::generateRandomNum(0, adjCountries.size() - 1);
        string randCountryName = adjCountries.at(randNum);
        int armyToMove = Game::getAllCountries().at(maxArmyCountry->getCountryName()).getNumOfArmy() - 1;
        Game::getAllCountries().at(randCountryName).addNumOfArmy(armyToMove);
        maxArmyCountry->reduceNumOfArmy(armyToMove);
    }
}

vector<Country *> Game::getPlayerCountries(int playerIndex) {
    vector<Country *> result;
    for (auto &item : getAllCountries()) {
        if (item.second.getOwnerIndex() == playerIndex) {
            result.push_back(&item.second);
        }
    }
    return result;
}

bool Game::hasAdjEnemyCountry(Country *pCountry) {
    vector<string> adjCountryNames = pCountry->getAdjacentCountires();
    for (auto &countryName: adjCountryNames) {
        Country &country = Game::getAllCountries().at(countryName);
        if (country.getOwnerIndex() != pCountry->getOwnerIndex()) {
            return true;
        }
    }
    return false;
}


void Game::saveMapConfigFromFile(string filePath) {

    ofstream outFile(filePath, ios_base::app);
    if (!outFile.is_open()) {
        cout << "Failed opening file from the path: " << filePath << endl;
    } else {

        outFile << endl;
        outFile << "[Players] " << endl;

        for (size_t i = 0; i < players.size(); i++) {

            string isCurrentPlayer = (players.at(i).getPlayerIndex() == curPlayerIndex) ? "True" : "False";
            vector<CardType> cards = players.at(i).getCards();
//            vector<Country> capturedCountries = players.at(i).getCapturedCountries();
            vector<string> capturedCountryNames = players.at(i).getCapturedCountryNames();

            outFile << "PlayerName=" << players.at(i).getPlayerName() << endl;

            outFile << "PlayerIndex=" << players.at(i).getPlayerIndex() << endl;

            outFile << "CurrentPlayer=" << isCurrentPlayer << endl;

            outFile << "Cards=";
            for (size_t i = 0; i < cards.size() - 1; i++) {
                outFile << cards.at(i) << ",";
            }
            outFile << cards.at(cards.size() - 1) << endl;

            outFile << "Captured Countries=";
            for (size_t i = 0; i < capturedCountryNames.size() - 1; i++) {
                outFile << capturedCountryNames.at(i) << ",";
            }
            outFile << capturedCountryNames.at(capturedCountryNames.size() - 1) << endl;

            outFile << "ContinentBonus=" << players.at(i).getContinentBonus() << endl;

            outFile << "Exchange Times=" << players.at(i).getExchangeTimes() << endl;

            if (isCurrentPlayer == "True") {
                outFile << "Stage=" << curGameStage << endl;
                if (curGameStage == DEPLOYMENT) {
                    outFile << "UndeployedArmyNumber=" << players.at(i).getUndeployArmyNumber() << endl;
                }
            }


        }

        outFile << "" << endl;

        outFile.close();
    }
}


void Game::loadGameConfigFromFile(string filePath) {
    fstream inFIle(filePath);
    if (!inFIle.is_open()) {
        cout << "Failed reading file from the path: " << filePath << endl;
    } else {
        string line;
        map<string, Country> allCountries;
        while (getline(inFIle, line)) {
            if (line.find(MapManager::CONTINENT_TITLE) != string::npos) {
                map<string, Continent> allContinents;
                while (getline(inFIle, line)) {
                    if (line.length() == 0) {
                        break;
                    } else {
                        stringstream ss(line);
                        string continentName;
                        string bonus;

                        getline(ss, continentName, '=');
                        getline(ss, bonus, '=');

                        Continent continent(continentName, stoi(bonus));
                        allContinents.insert({continentName, continent});
                    }
                }
                setAllContinents(allContinents);
            }

            if (line.find(MapManager::TERRITORIES_TITLE) != string::npos) {
                while (getline(inFIle, line)) {
                    if (line.length() == 0) {
                        continue;
                    } else {
                        stringstream ss(line);
                        vector<string> countryTokens;

                        while (ss.good()) {
                            string subString;
                            getline(ss, subString, ',');
                            countryTokens.push_back(subString);
                        }
                        ss.str("");
                        ss.clear();

                        string countryName = countryTokens.at(MapManager::COUNTRY_NAME_INDEX);
                        int coordinateX = stoi(countryTokens.at(MapManager::COUNTRY_COORDINATE_X)) *
                                          MapManager::IMAGE_WIDTH_RATIO;
                        int coordinateY = stoi(countryTokens.at(MapManager::COUNTRY_COORDINATE_Y)) *
                                          MapManager::IMAGE_HEIGHT_RATIO;
                        string continentName = countryTokens.at(MapManager::CONTINENT_NAME_INDEX);
                        int numOfArmy = stoi(countryTokens.at(MapManager::ARMY_NUMBER_INDEX));
                        getAllContinents().find(continentName)->second.addCountryName(countryName);

                        vector<string> adjacentCountries;
                        for (int i = MapManager::ADJACENT_COUNTRIES_STARTS; i < countryTokens.size(); i++) {
                            adjacentCountries.push_back(countryTokens.at(i));
                        }
                        Country country(countryName, coordinateX, coordinateY, adjacentCountries, numOfArmy);
                        country.setContinentName(continentName);
                        allCountries.insert({countryName, country});
                    }
                }
            }


            while (getline(inFIle, line)) {
                if (line.find(MapManager::PLAYER_TITLE) != string::npos) {
                    while (getline(inFIle, line)) {
                        if (line.length() == 0) {
                            break;
                        } else {
                            stringstream ss(line);

                            string playerName;
                            string playerIndex;

                            getline(ss, playerName, '=');
                            getline(ss, playerIndex, '=');

                            Player player(stoi(playerIndex), playerName);

                            string isCurrentPlayer;

                            getline(ss, isCurrentPlayer, ',');

                            string cardTokens;
                            vector<CardType> cards;

                            getline(ss, cardTokens, '=');
                            while (ss.good()) {
                                string subString;
                                getline(ss, subString, ',');
                                CardType card = MapManager::getCardTypeFromString(subString);
                                cards.push_back(card);
                            }
                            player.setCards(cards);

                            vector<string> capturedCountryNameTokens;
                            vector<Country> capturedCountries;
                            string countryName;
                            getline(ss, countryName, '=');
                            while (ss.good()) {
                                string subString;
                                getline(ss, subString, ',');
                                capturedCountryNameTokens.push_back(subString);
                            }
                            player.setCapturedCountries(capturedCountries);

                            string continentBonus;
                            string exchangeTimes;

                            getline(ss, continentBonus, '=');
                            getline(ss, exchangeTimes, '=');

                            player.addContinentBonus(stoi(continentBonus));
                            player.setExchangeTimes(stoi(exchangeTimes));

                            if (isCurrentPlayer == "True") {

                                curPlayerIndex = stoi(playerIndex);
                                string stage;
                                string undeployedArmyNumber;

                                getline(ss, stage, '=');
                                getline(ss, undeployedArmyNumber, '=');

                                curGameStage = MapManager::getGameStageFromString(stage);

                                player.setUndeployArmyNumber(stoi(undeployedArmyNumber));
                            }

                            players.push_back(player);
                        }

                    }

                }
                setAllCountries(allCountries);
            }
        }
    }
}