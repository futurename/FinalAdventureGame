#include "imports.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "ColorList.h"
#include <SDL_ttf.h>

class MapManager {
private:
    const static string DEFAULT_MAP;
    const static string DEFAULT_MAP_CONFIG;
    const static int SCREEN_WIDTH = 1600;
    const static int SCREEN_HEIGHT = 900;
    const static int MAP_VIEW_PORT_WIDTH = 1300;
    const static int MAP_VIEW_PORT_HEIGHT = SCREEN_HEIGHT;
    const static int TEXT_VIEW_PORT_WIDTH = SCREEN_WIDTH - MAP_VIEW_PORT_WIDTH;
    const static int TEXT_VIEW_PORT_HEIGHT = SCREEN_HEIGHT;
    const static int COUNTRY_MARK_LENGTH = 30;
    const static tuple<int, int, int, int> DEFAULT_BACKGROUND_COLOR;

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

    bool static SDLLoadMedia(string mapPath = DEFAULT_MAP);

    void static SDLClose();

    static Uint32 getPixel(SDL_Surface *surface, int x, int y);

    static SDL_Surface *loadSurface(std::string path);

public:
    static void readMapFromFile(string filename = DEFAULT_MAP);

    static void readMapConfigFromFile(string filename = DEFAULT_MAP_CONFIG);

    static void initMap(string mapName);

    static void start();

    static void renderMapViewPort();

    static void setOwnerColorMark(int centerX, int centerY, tuple<int, int, int, int> tuple);

    static void renderTextViewPort();

    static void renderMessage(int x, int y, const char *message, tuple<int, int, int, int> color, int fontSize,
                              const char *fontPath = "../Fonts/Rubik-ExtraBold.ttf");
};
