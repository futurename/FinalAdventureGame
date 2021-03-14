#include "imports.h"
#include "../Player.h"

class Game {
private:
    static vector<Player> players;

public:
    static vector<Player> getPlayers();
};
