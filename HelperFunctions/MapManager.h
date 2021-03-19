#include "imports.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include "Card.h"
#include "../Player.h"
#include "Game.h"

class Country;

enum ButtonType{LOAD, SAVE, RESET, NEXT, NONE};

class MapManager {
private:
    const static int SCREEN_WIDTH = 1800;
    const static int SCREEN_HEIGHT = 900;

    const static int MAP_VIEW_PORT_WIDTH = 1500;
    const static int MAP_VIEW_PORT_HEIGHT = SCREEN_HEIGHT;

    const static int PLAYER_INFO_HEIGHT = 150;
    const static int PLAYER_INFO_WIDTH = SCREEN_WIDTH - MAP_VIEW_PORT_WIDTH;
    const static int PLAYER_INFO_X = 60;
    const static int PLAYER_INFO_Y = 40;
    const static int PLAYER_INFO_SPACE = 120;
    const static int PLAYER_INFO_GAP = 25;
    const static int PLAYER_INFO_RECT_WIDTH = 15;
    const static int PLAYER_INFO_FONT_SIZE = 18;

    const static int COUNTRY_INFO_WIDTH = PLAYER_INFO_WIDTH;
    const static int COUNTRY_INFO_HEIGHT = 140;
    const static int COUNTRY_INFO_CENTER_X = COUNTRY_INFO_WIDTH / 2;
    const static int COUNTRY_INFO_START_Y = 15;
    const static int COUNTRY_INFO_GAP = 30;
    const static int COUNTRY_INFO_FONT_SIZE = 20;
    const static int COUNTRY_MARK_WIDTH = 74;
    const static int COUNTRY_MARK_HEIGHT = 55;
    const static int COUNTRY_TEXT_HEIGHT_SHIFT = 18;

    const static int NUMBER_LIST_WIDTH = PLAYER_INFO_WIDTH;
    const static int NUMBER_LIST_HEIGHT = 140;
    const static int NUMBER_LIST_X = 35;
    const static int NUMBER_LIST_GAP = 25;
    const static int NUMBER_LIST_ABSOLUTE_Y = PLAYER_INFO_HEIGHT + COUNTRY_INFO_HEIGHT;
    const static int NUMBER_LIST_SPACE = 50;
    const static int NUMBER_LIST_FONT_SIZE = 18;
    const static int NUMBER_LIST_MARK_WIDTH = 35;
    const static int NUMBER_LIST_UNDEPLOY_FONT_SIZE = 22;
    static vector<string> NUMBER_STRING_VECTOR;
    const static tuple<int, int, int, int> NUMBER_BACKGROUND_COLOR;
    const static tuple<int, int, int, int> NUMBER_TEXT_COLOR;
    static vector<SDL_Point> numberMarkCoordinates;

    const static int CARDS_LIST_WIDTH = PLAYER_INFO_WIDTH;
    const static int CARDS_LIST_HEIGHT = 300;
    const static int CARDS_LIST_ABSOLUTE_Y = PLAYER_INFO_HEIGHT + COUNTRY_INFO_HEIGHT + NUMBER_LIST_HEIGHT;
    const static int CARDS_LIST_IMAGE_HEIGHT = 80;
    const static int CARDS_LIST_IMAGE_WIDTH = 120;
    const static int CARDS_LIST_SPACE = 180;
    const static int CARDS_LIST_X = 40;
    const static int CARDS_LIST_Y = 20;
    const static int CARDS_LIST_GAP = 10;
    const static int CARDS_TEXT_GAP = 30;
    const static int CARDS_LIST_FONT_SIZE = 22;
    static tuple<int, int, int, int> CARDS_IMAGE_BORDER_COLOR;
    static vector<const char *> CARD_IMAGE_PATH_LIST;
    static vector<string> CARDS_TYPE_LIST;

    const static int BUTTONS_WIDTH = PLAYER_INFO_WIDTH;
    const static int BUTTONS_HEIGHT = 150;
    const static int BUTTONS_ABSOLUTE_Y =
            PLAYER_INFO_HEIGHT + COUNTRY_INFO_HEIGHT + NUMBER_LIST_HEIGHT + CARDS_LIST_HEIGHT;
    const static int BUTTONS_RECT_HEIGHT = 50;
    const static int BUTTONS_RECT_WIDTH = 100;
    const static int BUTTONS_GAP = 20;
    const static int BUTTONS_SPACE = 30;
    const static int BUTTONS_LEFT_X = 30;
    const static int BUTTONS_UPPER_Y = 20;
    const static int BUTTONS_TEXT_FONT_SIZE = 22;
    static vector<string> buttonNames;
    const static tuple<int, int, int, int> BUTTONS_BACKGROUND_COLOR;
    const static tuple<int, int, int, int> BUTTONS_BORDER_COLOR;
    static tuple<int, int, int, int> BUTTONS_TEXT_COLOR;
    const static char* BUTTONS_FONT_PATH;
    static map<ButtonType, SDL_Point> buttonCoordinates;

    const static tuple<int, int, int, int> DEFAULT_BACKGROUND_COLOR;

    const static int DEFAULT_MAP_FONT_SIZE = 14;

    const static char *DEFAULT_FONT_PATH;
    const static char *DEFAULT_TEXT_FONT_PATH;

    const static SDL_Rect countryInfoRect;
    const static SDL_Rect worldMapRect;
    const static SDL_Rect playerInfoRect;
    const static SDL_Rect numberListRect;
    const static SDL_Rect cardsListRect;
    const static SDL_Rect buttonsRect;

    static Country *fromCountry, *toCountry;

    const static SDL_MessageBoxColorScheme MESSAGE_BOX_COLOR_SCHEME;

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

    bool static SDLLoadMedia(string mapPath = Game::DEFAULT_MAP);

    void static SDLClose();

    static CardType getCardTypeFromString(string& cardStr);

public:
    static void initCountryMarks();

    static void start(string mapPath = Game::DEFAULT_MAP);

    static void initMapImage();

    static void setOwnerColorMark(int centerX, int centerY, tuple<int, int, int, int> color);

    static void initTextViewPort();

    static void renderMessage(int x, int y, const char *message, tuple<int, int, int, int> color, int fontSize,
                              const char *fontPath = DEFAULT_FONT_PATH);

    static void detectImageWidthHeightRatio(string &mapPath);

    static string getCountryNameFromCoordinates(int x, int y);

    static void renderCountryMark(int x, int y, Country &country, int fontSize);

    static void updateCountryInfoRect(vector<string> &messages, tuple<int, int, int, int> color);

    static void updateMapRect();

    static void resetToDefaultColor();

    static void renderPlayerInfoRect();

    static void renderCountryInfoRect(Country *pickedCountry);

    static void clearCountryInfoRect();

    static void updateWholeScreen();

    static void renderNumberListRect();

    static int getIndexOfDraggedNumber(SDL_Point point);

    static bool isDragToOwnCountry(SDL_Point point, int index);

    static void renderCardsListRect();

    static void renderButtonsRect();

    static void loadGameFromFile();

    static ButtonType clickButtonType(SDL_Point& p);

    static void saveGameToFile();

    static void resetGame();

    static void nextStage();

    static ButtonType getButtonTypeFromStr(const string& buttonName);

    const static int ADJACENT_COUNTRIES_STARTS = 5;
    const static int ARMY_NUMBER_INDEX = 3;
    const static int CONTINENT_NAME_INDEX = 4;
    const static string CONTINENT_TITLE;
    const static int COUNTRY_COORDINATE_X = 1;
    const static int COUNTRY_COORDINATE_Y = 2;
    const static int COUNTRY_NAME_INDEX = 0;
    static double IMAGE_HEIGHT_RATIO;
    static double IMAGE_WIDTH_RATIO;
    const static string TERRITORIES_TITLE;

    static bool isAdjacentCountry(Country *attackingCountry, Country *defendingCountry);

    static bool canAttackFromAnyCountry(Player &player);

    static bool isSameOwner(const string& fromCountryName, const string& toCountryName);
};
