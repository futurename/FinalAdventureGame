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

    const static int MAP_VIEW_PORT_WIDTH = 1500;
    const static int MAP_VIEW_PORT_HEIGHT = SCREEN_HEIGHT;

    const static int PLAYER_VIEWPORT_HEIGHT = 180;
    const static int PLAYER_VIEWPORT_WIDTH = SCREEN_WIDTH - MAP_VIEW_PORT_WIDTH;
    const static int PLAYER_INFO_X = 60;
    const static int PLAYER_INFO_Y = 40;
    const static int PLAYER_INFO_SPACE = 120;
    const static int PLAYER_INFO_GAP = 25;
    const static int PLAYER_INFO_RECT_WIDTH = 15;
    const static int PLAYER_INFO_FONT_SIZE = 18;

    const static int COUNTRY_INFO_WIDTH = PLAYER_VIEWPORT_WIDTH;
    const static int COUNTRY_INFO_HEIGHT = SCREEN_HEIGHT - PLAYER_VIEWPORT_HEIGHT;
    const static int COUNTRY_INFO_CENTER_X = COUNTRY_INFO_WIDTH / 2;
    const static int COUNTRY_INFO_START_Y = 15;
    const static int COUNTRY_INFO_GAP = 30;
    const static int COUNTRY_INFO_FONT_SIZE = 20;

    const static int COUNTRY_MARK_WIDTH = 74;
    const static int COUNTRY_MARK_HEIGHT = 55;
    const static int COUNTRY_TEXT_HEIGHT_SHIFT = 18;

    const static tuple<int, int, int, int> DEFAULT_BACKGROUND_COLOR;
    const static string DEFAULT_MAP;
    const static string DEFAULT_MAP_CONFIG;
    const static string TERRITORIES_TITLE;
    const static string CONTINENT_TITLE;

    const static int DEFAULT_MAP_FONT_SIZE = 14;

    const static int COUNTRY_NAME_INDEX = 0;
    const static int COUNTRY_COORDINATE_X = 1;
    const static int COUNTRY_COORDINATE_Y = 2;
    const static int ARYM_NUMBER_INDEX = 3;
    const static int CONTINENT_NAME_INDEX = 4;
    const static int ADJACENT_COUNTRIES_STARTS = 5;

    const static char *DEFAULT_FONT_PATH;
    const static char *DEFAULT_TEXT_FONT_PATH;
    static double IMAGE_WIDTH_RATIO;
    static double IMAGE_HEIGHT_RATIO;
    static SDL_Rect textViewPort;
    static SDL_Rect mapViewPort;
    static SDL_Rect playerViewPort;

    //Loads individual image as texture
    static SDL_Texture *loadTexture(std::string path);

    //The window we'll be rendering to
    static SDL_Window *gWindow;

    //The map renderer
    static SDL_Renderer *gRenderer;

    //Current displayed texture
    static SDL_Texture *gMapTexture;

    //Font used for display
    static TTF_Font *gFont;

    bool static SDLInit();

    bool static SDLLoadMedia(string mapPath = DEFAULT_MAP);

    void static SDLClose();

public:
    static void initWorldMarks();

    static void start(string mapPath = DEFAULT_MAP);

    static void renderWorldMap();

    static void setOwnerColorMark(int centerX, int centerY, tuple<int, int, int, int> color);

    static void initTextViewPort();

    static void renderMessage(int x, int y, const char *message, tuple<int, int, int, int> color, int fontSize,
                              const char *fontPath = DEFAULT_FONT_PATH);

    static void readMapConfigFromFile(string filePath = DEFAULT_MAP_CONFIG);

    static void detectImageWidthHeightRatio(string &mapPath);

    static string getCountryNameFromCoordinates(int x, int y);

    static void renderCountryMark(int x, int y, Country &country, const int fontSize);

    static void updateTextViewPort(vector<string> &messages, tuple<int, int, int, int> color);

    static void updateWorldMap();

    static void resetToDefaultColor();

    static void renderPlayerInfo();

    static void renderCountryInfo(Country *pickedCountry);

    static void clearCountryInfo();

    static void updateWholeScreen();
};
