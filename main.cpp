
#include "HelperFunctions//MapManager.h"
#include "HelperFunctions/Game.h"
#include "Random.h"

int main(int argc, char *args[]) {
    srand((unsigned) time(nullptr));

    MapManager::start("../Maps/World.bmp");
/*
      MapManager::readMapConfigFromFile();
      Game::printAllContinents();

      Game::printAllCountries();*/

 /* pair<int,int> onePair1 = Random::RollDie(1);
  pair<int,int> onePair2 = Random::RollDie(2);
  pair<int,int> onePair3 = Random::RollDie(3);

  cout << onePair1.first << ", " << onePair1.second << endl;
  cout << onePair2.first << ", " << onePair2.second << endl;
  cout << onePair3.first << ", " << onePair3.second << endl;*/

    /*Game::initPlayersAndCountries();
    Game::printAllCountries();
    Game::printAllContinents();*/

    return 0;
}
