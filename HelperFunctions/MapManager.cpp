#include "MapManager.h"
#include "Game.h"
#include "ColorList.h"
#include "../Country.h"

SDL_Window *MapManager::gWindow = NULL;

SDL_Renderer *MapManager::gRenderer = NULL;

SDL_Texture *MapManager::gMapTexture = NULL;

SDL_Surface *MapManager::gScreenSurface = NULL;

TTF_Font *MapManager::gFont = NULL;

const string MapManager::DEFAULT_MAP = "../Maps/World.bmp";

const string MapManager::DEFAULT_MAP_CONFIG = "../Maps/World.map";

const string MapManager::TERRITORIES_HEADER = "Territories";

const char *MapManager::DEFAULT_FONT_PATH = "../Fonts/FiraSans-Regular.ttf";

const tuple<int, int, int, int> MapManager::DEFAULT_BACKGROUND_COLOR = ColorList::WHITE;

map<string, SDL_Texture *> MapManager::countryTextures{map<string, SDL_Texture *>()};

double MapManager::IMAGE_WIDTH_RATIO = 1.0;

double MapManager::IMAGE_HEIGHT_RATIO = 1.0;

void MapManager::initWorldMarks() {
    readMapConfigFromFile();

    map<string, Country> allCountries = Game::getAllCountries();

    for (auto itr = allCountries.begin(); itr != allCountries.end(); itr++) {
        Country &country = itr->second;
        int x = country.getX();
        int y = country.getY();

        setOwnerColorMark(x, y, country.getCountryColour());

        renderCountryMark(x, y, country, COUNTRY_NAME_FONT_SIZE);

    }
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
                tuple<int, int, int, int> bgColor = ColorList::WHITE;
                SDL_SetRenderDrawColor(gRenderer, get<0>(bgColor), get<1>(bgColor), get<2>(bgColor),
                                       get<3>(bgColor));

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                //Initialize SDL_ttf
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool MapManager::SDLLoadMedia(string mapPath) {
    //Loading success flag
    bool success = true;

    //Load PNG texture
    gMapTexture = loadTexture(mapPath);
    if (gMapTexture == NULL) {
        printf("Failed to load texture image!\n");
        success = false;
    }
    return success;
}

void MapManager::SDLClose() {
    //Free loaded image
    SDL_DestroyTexture(gMapTexture);
    gMapTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Free global font
    TTF_CloseFont(gFont);
    gFont = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *MapManager::loadTexture(string path) {
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

void MapManager::start(string mapPath) {
    //Start up SDL and create window
    if (!SDLInit()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (!SDLLoadMedia(mapPath)) {
            printf("Failed to load media!\n");
        } else {
            //Main loop flag
            bool quit = false;

            SDL_RenderClear(gRenderer);

            //get raw image width/height vs display ratio
            detectImageWidthHeightRatio(mapPath);

            //*******************************************
            //Rendering map vew port
            renderMapViewPort();

            initWorldMarks();

            /*tuple<int, int, int, int> paintColor = ColorList::BLUE;
            setOwnerColorMark(300, 400, paintColor);*/



            //*******************************************
            //rendering text view port
            renderTextViewPort();



            //******************************************
            //render font


            //Update screen
            SDL_RenderPresent(gRenderer);



            //Event handler
            SDL_Event e;

            //While application is running
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    string message;
                    Country *pickedCountry;

                    switch (e.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;
                        case SDL_MOUSEMOTION:
                            message = to_string(e.motion.x) + ":" + to_string(e.motion.y);
                            if (!getCountryNameFromCoordinates(e.motion.x, e.motion.y).empty()) {
                                message = getCountryNameFromCoordinates(e.motion.x, e.motion.y);
                                pickedCountry = &Game::getAllCountries().find(message.c_str())->second;
                                clearTextViewPort();
                                renderMapViewPort(*pickedCountry);
                                renderCountryMark(pickedCountry->getX(), pickedCountry->getY(), *pickedCountry, 22);
                            }
                            SDL_SetWindowTitle(gWindow, message.c_str());
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            switch (e.button.button) {
                                case SDL_BUTTON_LEFT:
                                    message = getCountryNameFromCoordinates(e.motion.x, e.motion.y);
                                    SDL_ShowSimpleMessageBox(0, "check range", message.c_str(), gWindow);
                                    break;
                            }

                    }
                }
            }
        }
    }

    //Free resources and close SDL
    SDLClose();
}

void MapManager::renderMapViewPort() {
    SDL_Rect mapViewPort;
    mapViewPort.x = 0;
    mapViewPort.y = 0;
    mapViewPort.w = MAP_VIEW_PORT_WIDTH;
    mapViewPort.h = SCREEN_HEIGHT;
    SDL_RenderSetViewport(gRenderer, &mapViewPort);

    SDL_RenderCopy(gRenderer, gMapTexture, NULL, NULL);

    renderMessage(0, 10, "Hello world", ColorList::RED, 18);
}

void MapManager::setOwnerColorMark(int centerX, int centerY, tuple<int, int, int, int> color) {
    SDL_SetRenderDrawColor(gRenderer, get<0>(color), get<1>(color), get<2>(color),
                           get<3>(color));
    SDL_Rect sdlRect;
    sdlRect.x = centerX - COUNTRY_MARK_WIDTH / 2;
    sdlRect.y = centerY - COUNTRY_MARK_HEIGHT / 2;
    sdlRect.w = COUNTRY_MARK_WIDTH;
    sdlRect.h = COUNTRY_MARK_HEIGHT;

    SDL_RenderFillRect(gRenderer, &sdlRect);

    SDL_SetRenderDrawColor(gRenderer, get<0>(DEFAULT_BACKGROUND_COLOR),
                           get<1>(DEFAULT_BACKGROUND_COLOR),
                           get<2>(DEFAULT_BACKGROUND_COLOR),
                           get<3>(DEFAULT_BACKGROUND_COLOR));
}

void MapManager::renderTextViewPort() {
    SDL_Rect textVeiewPort{MAP_VIEW_PORT_WIDTH, 0, SCREEN_WIDTH - MAP_VIEW_PORT_WIDTH, SCREEN_HEIGHT};

    SDL_RenderSetViewport(gRenderer, &textVeiewPort);

    tuple<int, int, int, int> bgColor = ColorList::LIGHTER_YELLOW;
    SDL_SetRenderDrawColor(gRenderer, get<0>(bgColor), get<1>(bgColor), get<2>(bgColor), get<3>(bgColor));

    SDL_RenderFillRect(gRenderer, nullptr);

    tuple<int, int, int, int> borderColor = ColorList::BLACK;
    SDL_SetRenderDrawColor(gRenderer, get<0>(borderColor), get<1>(borderColor), get<2>(borderColor),
                           get<3>(borderColor));

    SDL_RenderDrawRect(gRenderer, nullptr);

    renderMessage(0, 10, "Hello world", ColorList::RED, 18);
}

void MapManager::renderMapViewPort(Country &country) {
    SDL_Rect textVeiewPort{MAP_VIEW_PORT_WIDTH, 0, SCREEN_WIDTH - MAP_VIEW_PORT_WIDTH, SCREEN_HEIGHT / 2};

    SDL_RenderSetViewport(gRenderer, &textVeiewPort);

    tuple<int, int, int, int> bgColor = ColorList::LIGHTER_YELLOW;
    SDL_SetRenderDrawColor(gRenderer, get<0>(bgColor), get<1>(bgColor), get<2>(bgColor), get<3>(bgColor));

    SDL_RenderFillRect(gRenderer, nullptr);

    tuple<int, int, int, int> borderColor = ColorList::BLACK;
    SDL_SetRenderDrawColor(gRenderer, get<0>(borderColor), get<1>(borderColor), get<2>(borderColor),
                           get<3>(borderColor));

    SDL_RenderDrawRect(gRenderer, nullptr);

    renderMessage(0, 10, "Hello world", ColorList::RED, 18);

    renderMessage(50, 50, country.getCountryName().c_str(), ColorList::RED, 18);
    renderMessage(50, 80, to_string(country.getCountryArmy()).c_str(), ColorList::RED, 18);

    SDL_RenderCopy(gRenderer, gMapTexture, NULL, NULL);
}

void MapManager::renderMessage(int x, int y, const char *message, tuple<int, int, int, int> color, int fontSize,
                               const char *fontPath) {
    SDL_Surface *textSurface;
    gFont = TTF_OpenFont(fontPath, fontSize);

    SDL_Color textColor = {(Uint8) get<0>(color), (Uint8) get<1>(color),
                           (Uint8) get<2>(color)};

    textSurface = TTF_RenderText_Solid(gFont, message, textColor);
    if (!textSurface) {
        cout << "Failed to render textSurface: " << TTF_GetError() << endl;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

    SDL_SetRenderTarget(gRenderer, textTexture);
    /*SDL_BlendMode blending;
    SDL_SetTextureBlendMode(textTexture, blending);*/

    SDL_Rect dest = {x - textSurface->w / 2, y - textSurface->h / 2, textSurface->w, textSurface->h};

    SDL_RenderCopy(gRenderer, textTexture, NULL, &dest);

    SDL_SetRenderTarget(gRenderer, NULL);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void MapManager::readMapConfigFromFile(string filePath) {
    fstream inFIle(filePath);
    if (!inFIle.is_open()) {
        cout << "Failed reading file from the path: " << filePath << endl;
    } else {
        string line;
        map<string, Country> allCountries;
        while (getline(inFIle, line)) {
            if (line.find(TERRITORIES_HEADER) != string::npos) {
                while (getline(inFIle, line)) {
                    if (line.length() == 0) {

                        //FIXME different continent
                        continue;
                    } else {
                        stringstream ss(line);
                        vector<string> countryTokens;
                        while (ss.good()) {
                            string subString;
                            getline(ss, subString, ',');
                            countryTokens.push_back(subString);
                        }
                        ss.str("");
                        ss.clear();

                        string countryName = countryTokens.at(COUNTRY_NAME_INDEX);
                        int coordinateX = stoi(countryTokens.at(COUNTRY_COORDINATE_X)) * IMAGE_WIDTH_RATIO;
                        int coordinateY = stoi(countryTokens.at(COUNTRY_COORDINATE_Y)) * IMAGE_HEIGHT_RATIO;
                        vector<string> adjacentCountries;
                        for (int i = ADJACENT_COUNTRIES_STARTS; i < countryTokens.size(); i++) {
                            adjacentCountries.push_back(countryTokens.at(i));
                        }
                        Country country(countryName, coordinateX, coordinateY, adjacentCountries);
                        allCountries.insert({countryName, country});
                    }
                }
            }
        }
        Game::setAllCountries(allCountries);
    }
}

void MapManager::detectImageWidthHeightRatio(string &mapPath) {
    SDL_Surface *surface = IMG_Load(mapPath.c_str());
    IMAGE_WIDTH_RATIO = (double) MAP_VIEW_PORT_WIDTH / surface->w;
    IMAGE_HEIGHT_RATIO = (double) MAP_VIEW_PORT_HEIGHT / surface->h;
}

string MapManager::getCountryNameFromCoordinates(int x, int y) {
    for (auto itr = Game::getAllCountries().begin(); itr != Game::getAllCountries().end(); itr++) {
        int coorX = itr->second.getX();
        int coorY = itr->second.getY();
        int widthDia = COUNTRY_MARK_WIDTH / 2;
        int heightDia = COUNTRY_MARK_HEIGHT / 2;
        if (x <= coorX + widthDia && x >= coorX - widthDia && y >= coorY - heightDia && y <= coorY + heightDia) {
            return itr->first;
        }
    }
    return "";
}

void MapManager::renderCountryMark(int x, int y, Country &country, const int fontSize) {
    renderMessage(x, y - COUNTRY_TEXT_HEIGHT_SHIFT, country.getCountryName().c_str(), ColorList::RED,
                  COUNTRY_NAME_FONT_SIZE);
    renderMessage(x, y + COUNTRY_TEXT_HEIGHT_SHIFT, to_string(country.getCountryArmy()).c_str(), ColorList::RED,
                  COUNTRY_NAME_FONT_SIZE);
}

void MapManager::clearTextViewPort() {

    SDL_Texture *textTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                 SCREEN_WIDTH - MAP_VIEW_PORT_WIDTH, SCREEN_HEIGHT);


    SDL_RenderCopy(gRenderer, textTexture, NULL, NULL);

}

