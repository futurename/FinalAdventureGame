#include "imports.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "ColorList.h"

using namespace std;

class MapManager {
private:
    const static string DEFAULT_MAP;
    const static string DEFAULT_MAP_CONFIG;
    const static int SCREEN_WIDTH = 1600;
    const static int SCREEN_HEIGHT = 900;

    //Loads individual image as texture
    static SDL_Texture *loadTexture(std::string path);

    //The window we'll be rendering to
    static SDL_Window *gWindow;

    //The window renderer
    static SDL_Renderer *gRenderer;

    //Current displayed texture
    static SDL_Texture *gTexture;

    //The surface contained by the window
    static SDL_Surface* gScreenSurface;

    //Current displayed image
    static SDL_Surface* gStretchedSurface;

    bool static SDLInit();

    bool static SDLLoadMediaToTexture(string mapPath = DEFAULT_MAP);

    void static SDLCloseForTexture();

    static Uint32 getPixel(SDL_Surface *surface, int x, int y);

    static SDL_Surface* loadSurface( std::string path );

public:
    static void readMapFromFile(string filename = DEFAULT_MAP);

    static void readMapConfigFromFile(string filename = DEFAULT_MAP_CONFIG);

    static void initMap(string mapName);

    static void start();


};
