
#include "HelperFunctions//MapManager.h"
#include "HelperFunctions/Game.h"

using namespace std;

int main(int argc, char *args[]) {

    //MapManager::start("../Maps/World.bmp");

    MapManager::readMapConfigFromFile();
    Game::printAllContinents();

    Game::printAllCountries();

    return 0;
}
