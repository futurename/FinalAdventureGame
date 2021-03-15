#include "imports.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>

class Country;

class MapManager {
private:
    const static int SCREEN_WIDTH = 1800;
    const static int SCREEN_HEIGHT = 900;
    const static int MAP_VIEW_PORT_WIDTH = 1400;
    const static int MAP_VIEW_PORT_HEIGHT = SCREEN_HEIGHT;
    const static int TEXT_VIEW_PORT_WIDTH = SCREEN_WIDTH - MAP_VIEW_PORT_WIDTH;
    const static int TEXT_VIEW_PORT_HEIGHT = SCREEN_HEIGHT;
    const static int COUNTRY_MARK_WIDTH = 70;
    const static int COUNTRY_MARK_HEIGHT = 50;
    const static int COUNTRY_TEXT_HEIGHT_SHIFT = 10;
    const static int COUNTRY_NAME_FONT_SIZE = 14;
    const static tuple<int, int, int, int> DEFAULT_BACKGROUND_COLOR;
    const static string DEFAULT_MAP;
    const static string DEFAULT_MAP_CONFIG;
    const static string TERRITORIES_HEADER;
    const static int COUNTRY_NAME_INDEX = 0;
    const static int COUNTRY_COORDINATE_X = 1;
    const static int COUNTRY_COORDINATE_Y = 2;
    const static int ADJACENT_COUNTRIES_STARTS = 3;
    const static char* DEFAULT_FONT_PATH;
    static double IMAGE_WIDTH_RATIO;
    static double IMAGE_HEIGHT_RATIO;

    //Loads individual image as texture
    static SDL_Texture *loadTexture(std::string path);

    //The window we'll be rendering to
    static SDL_Window *gWindow;

    //The map renderer
    static SDL_Renderer *gRenderer;

    //Current displayed texture
    static SDL_Texture *gMapTexture;

    //The surface contained by the window
    static SDL_Surface *gScreenSurface;

    //Font used for display
    static TTF_Font *gFont;

    bool static SDLInit();

    bool static SDLLoadMedia(const string mapPath = DEFAULT_MAP);

    void static SDLClose();

public:
    static void initWorldMarks();

    static void start(string mapPath);

    static void renderMapViewPort();

    static void setOwnerColorMark(int centerX, int centerY, tuple<int, int, int, int> color);

    static void renderTextViewPort();

    static void renderMessage(int x, int y, const char *message, tuple<int, int, int, int> color, int fontSize,
                              const char *fontPath = DEFAULT_FONT_PATH);

    static void readMapConfigFromFile(string filePath = DEFAULT_MAP_CONFIG);

    static void detectImageWidthHeightRatio(string& mapPath);

    static string getCountryNameWhenMouseClick(int x, int y);

    static void renderCountryMark(int x, int y, Country &country, const int fontSize);
};
