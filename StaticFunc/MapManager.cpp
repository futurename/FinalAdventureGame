
#include "MapManager.h"


using namespace std;

const string MapManager::DEFAULT_MAP = "../maps/World.bmp";

const string MapManager::DEFAULT_MAP_CONFIG = "../maps/World.map";

SDL_Window *MapManager::gWindow = NULL;

SDL_Renderer *MapManager::gRenderer = NULL;

SDL_Texture *MapManager::gTexture = NULL;

SDL_Surface *MapManager::gScreenSurface = NULL;

SDL_Surface *MapManager::gStretchedSurface = NULL;

void MapManager::readMapFromFile(string filename) {

}

void MapManager::readMapConfigFromFile(string filename) {

}

void MapManager::initMap(string mapName) {

}

bool MapManager::SDLInit() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool MapManager::SDLLoadMediaToTexture(string mapPath) {
    //Loading success flag
    bool success = true;

    //Load PNG texture
    gTexture = loadTexture(mapPath);
    if (gTexture == NULL) {
        printf("Failed to load texture image!\n");
        success = false;
    }

    return success;
}

SDL_Surface *MapManager::loadSurface(std::string path) {
    //The final optimized image
    SDL_Surface *optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

void MapManager::SDLCloseForTexture() {
    //Free loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *MapManager::loadTexture(std::string path) {
    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

Uint32 MapManager::getPixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *) p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *) p;
        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void MapManager::start() {
    //Start up SDL and create window
    if (!SDLInit()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (!SDLLoadMediaToTexture()) {
            printf("Failed to load media!\n");
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //While application is running
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                //Clear screen
                //SDL_RenderClear( gRenderer );

                tuple<int,int,int,int> red = ColorList::getBlue();
                SDL_SetRenderDrawColor(gRenderer, get<0>(red), get<1>(red), get<2>(red), get<3>(red));


                //Render texture to screen
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);


                for (int i = 200; i < 400; i++) {
                    for (int j = 300; j < 400; j++) {
                        SDL_RenderDrawPoint(gRenderer, i, j);
                    }
                }


                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    SDLCloseForTexture();
}

