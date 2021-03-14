#include <tuple>
#include "imports.h"

class ColorList {
private:
    static tuple<int, int, int, int> RED;
    static tuple<int, int, int, int> GREEN;
    static tuple<int, int, int, int> BLUE;
    static tuple<int, int, int, int> YELLOW;
public:
    static tuple<int, int, int, int> getRed();

    static tuple<int, int, int, int> getGreen();

    static tuple<int, int, int, int> getBlue();

    static tuple<int, int, int, int> getYellow();
};
