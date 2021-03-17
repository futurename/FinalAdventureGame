#include <tuple>
#include "imports.h"

class ColorList {
private:
    static vector<pair<tuple<int, int, int, int>, tuple<int, int, int, int>>> colorPairs;
public:
    const static tuple<int, int, int, int> RED;
    const static tuple<int, int, int, int> GREEN;
    const static tuple<int, int, int, int> BLUE;
    const static tuple<int, int, int, int> YELLOW;
    const static tuple<int, int, int, int> WHITE;
    const static tuple<int, int, int, int> BLACK;
    const static tuple<int, int, int, int> LIGHTER_YELLOW;
    const static tuple<int, int, int, int> PURPLE;
    const static tuple<int, int, int, int> CYAN;
    const static tuple<int, int, int, int> PUMPKIN;
    const static tuple<int, int, int, int> INDIGO;

    void generateColorPairs(int numOfPlayers);

    static vector<pair<tuple<int, int, int, int>, tuple<int, int, int, int>>> getPlayerColorPairs();
};