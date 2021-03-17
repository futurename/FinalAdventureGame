#include "MapManager.h"
#include "Game.h"
#include "ColorList.h"

SDL_Window *MapManager::gWindow = NULL;

SDL_Renderer *MapManager::gRenderer = NULL;

SDL_Texture *MapManager::gMapTexture = NULL;

TTF_Font *MapManager::gFont = NULL;

const string MapManager::DEFAULT_MAP = "../Maps/World.bmp";

const string MapManager::DEFAULT_MAP_CONFIG = "../Maps/World.map";

const string MapManager::TERRITORIES_TITLE = "Territories";

const string MapManager::CONTINENT_TITLE = "Continents";

const char *MapManager::DEFAULT_FONT_PATH = "../Fonts/FiraSans-Regular.ttf";

const char *MapManager::DEFAULT_TEXT_FONT_PATH = "../Fonts/NotoSansTC-Bold.otf";

const tuple<int, int, int, int> MapManager::DEFAULT_BACKGROUND_COLOR = ColorList::WHITE;

const vector<string> MapManager::NUMBER_STRING_VECTOR{"1", "2", "5", "10", "ALL"};

const tuple<int, int, int, int> MapManager::NUMBER_BACKGROUND_COLOR{ColorList::BLACK};

const tuple<int, int, int, int> MapManager::NUMBER_TEXT_COLOR{ColorList::WHITE};

double MapManager::IMAGE_WIDTH_RATIO = 1.0;

double MapManager::IMAGE_HEIGHT_RATIO = 1.0;

SDL_Rect MapManager::countryInfoRect{MAP_VIEW_PORT_WIDTH, PLAYER_INFO_HEIGHT, COUNTRY_INFO_WIDTH,
                                     COUNTRY_INFO_HEIGHT};

SDL_Rect MapManager::worldMapRect{0, 0, MAP_VIEW_PORT_WIDTH, MAP_VIEW_PORT_HEIGHT};

SDL_Rect MapManager::playerInfoRect{MAP_VIEW_PORT_WIDTH, 0, PLAYER_INFO_WIDTH, PLAYER_INFO_HEIGHT};

SDL_Rect MapManager::numberListRect{MAP_VIEW_PORT_WIDTH, NUMBER_LIST_ABSOLUTE_Y, NUMBER_LIST_WIDTH, NUMBER_LIST_HEIGHT};

vector<SDL_Point> MapManager::numberMarkCoordinates{vector<SDL_Point>()};

void MapManager::initWorldMarks() {
    readMapConfigFromFile();

    Game::initPlayersAndCountries();

    map<string, Country> allCountries = Game::getAllCountries();

    for (auto &item : allCountries) {
        Country &country = item.second;
        int x = country.getX();
        int y = country.getY();

        setOwnerColorMark(x, y, country.getCountryColour());

        renderCountryMark(x, y, country, DEFAULT_MAP_FONT_SIZE);
    }
    resetToDefaultColor();
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

            //get raw image width/height vs display ratio
            detectImageWidthHeightRatio(mapPath);

            SDL_RenderClear(gRenderer);

            //*******************************************
            //Rendering map vew port
            renderWorldMap();

            initWorldMarks();

            //*******************************************
            //rendering text view port
            initTextViewPort();

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
                    Country *clickedCountry;
                    SDL_Point dragStartPoint, dragEndPoint;
                    stringstream ss;
                    Country *fromCountry, *toCountry;

                    Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());

                    switch (e.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;

                        case SDL_MOUSEMOTION:
                            message = to_string(e.motion.x) + ":" + to_string(e.motion.y);
                            if (!getCountryNameFromCoordinates(e.motion.x, e.motion.y).empty()) {
                                message = getCountryNameFromCoordinates(e.motion.x, e.motion.y);
                                clickedCountry = &Game::getAllCountries().find(message)->second;
                                renderCountryInfo(clickedCountry);
                            } else {
                                clickedCountry = nullptr;
                                clearCountryInfo();
                            }
                            SDL_SetWindowTitle(gWindow, message.c_str());

                            break;

                        case SDL_MOUSEBUTTONDOWN:
                            switch (e.button.button) {
                                case SDL_BUTTON_LEFT:
                                    dragStartPoint = {e.motion.x, e.motion.y};
                                    if (clickedCountry != nullptr) {
                                        renderCountryInfo(clickedCountry);

                                        updateWholeScreen();
                                    }
                                    break;
                            }
                        case SDL_MOUSEBUTTONUP:
                            switch (e.button.button) {
                                case SDL_BUTTON_LEFT:
                                    dragEndPoint = {e.motion.x, e.motion.y};

                                    string fromCountryName = getCountryNameFromCoordinates(dragStartPoint.x,
                                                                                           dragStartPoint.y);
                                    if (!fromCountryName.empty()) {
                                        fromCountry = &Game::getAllCountries().find(fromCountryName)->second;
                                    } else {
                                        fromCountry = nullptr;
                                    }
                                    string toCountryName = getCountryNameFromCoordinates(dragEndPoint.x,
                                                                                         dragEndPoint.y);
                                    if (!toCountryName.empty()) {
                                        toCountry = &Game::getAllCountries().find(toCountryName)->second;
                                    } else {
                                        toCountry = nullptr;
                                    }

                                    if (fromCountry != nullptr && toCountry != nullptr &&
                                        fromCountryName != toCountryName) {

                                        //CHEATING mode for testing attack function.
                                        if (fromCountry->getOwnerIndex() != toCountry->getOwnerIndex()) {
                                            /*  ss.str("");
                                              ss.clear();
                                              ss << "From <" << fromCountryName << "> to <" << toCountryName << ">";
                                              SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT, "Drag",
                                                                       ss.str().c_str(), NULL);*/
                                            Game::conquerTheCountry(*fromCountry, *toCountry);

                                            updateWholeScreen();
                                            // updateWorldMap();
/*
                                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT, "Result",
                                                                     to_string(toCountry->getOwnerIndex()).c_str(), NULL);*/
                                        }
                                    }

                                    //deploy numbers
                                    if (isDragFromNumber(dragStartPoint) && isDragToOwnCountry(dragEndPoint, curPlayer.getPlayerIndex())) {
                                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT, "Drag",
                                                                 "Own country", NULL);

                                        if(Game::getGameStage() == DEPLOYMENT){
                                            //FIXME
                                        }

                                        if(Game::getGameStage() == MOVE){
                                            //FIXME
                                        }
                                    }





                                    //FIXME attack state

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

void MapManager::renderCountryInfo(Country *pickedCountry) {
    vector<string> messages;
    stringstream ss;
    ss << "Country:  " << pickedCountry->getCountryName();

    messages.push_back(ss.str());
    Player &player = Game::getAllPlayers().at(pickedCountry->getOwnerIndex());
    ss.str("");
    ss << "Player:  " << player.getPlayerName();
    messages.push_back(ss.str());

    ss.str("");
    ss << "Continent:  " << pickedCountry->getContinentName();
    messages.push_back(ss.str());

    ss.str("");
    ss << "Army:  " << to_string(pickedCountry->getCountryArmy());
    messages.push_back(ss.str());

    ss.str("");
    updateTextViewPort(messages, player.getTextColor());
}

void MapManager::renderWorldMap() {
    //SDL_RenderSetViewport(gRenderer, &worldMapRect);
    SDL_RenderCopy(gRenderer, gMapTexture, NULL, &worldMapRect);
    SDL_RenderPresent(gRenderer);
    resetToDefaultColor();
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
}

void MapManager::initTextViewPort() {
    renderPlayerInfo();
    resetToDefaultColor();
    rednerNumberList(true, 7);
}

void MapManager::renderMessage(int x, int y, const char *message, tuple<int, int, int, int> color, int fontSize,
                               const char *fontPath) {
    gFont = TTF_OpenFont(fontPath, fontSize);

    SDL_Color textColor = {(Uint8) get<0>(color), (Uint8) get<1>(color),
                           (Uint8) get<2>(color)};

    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, message, textColor);
    if (!textSurface) {
        cout << "Failed to render textSurface: " << TTF_GetError() << endl;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

    //SDL_SetRenderTarget(gRenderer, textTexture);
    /*SDL_BlendMode blending;
    SDL_SetTextureBlendMode(textTexture, blending);*/

    SDL_Rect dest = {x - textSurface->w / 2, y - textSurface->h / 2, textSurface->w, textSurface->h};

    SDL_RenderCopy(gRenderer, textTexture, NULL, &dest);

    // SDL_SetRenderTarget(gRenderer, NULL);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    resetToDefaultColor();
}

void MapManager::readMapConfigFromFile(string filePath) {
    fstream inFIle(filePath);
    if (!inFIle.is_open()) {
        cout << "Failed reading file from the path: " << filePath << endl;
    } else {
        string line;
        map<string, Country> allCountries;
        while (getline(inFIle, line)) {
            if (line.find(CONTINENT_TITLE) != string::npos) {
                map<string, Continent> allContinents;
                while (getline(inFIle, line)) {
                    if (line.length() == 0) {
                        break;
                    } else {
                        stringstream ss(line);
                        string continentName;
                        string bonus;

                        getline(ss, continentName, '=');
                        getline(ss, bonus, '=');

                        Continent continent(continentName, stoi(bonus));
                        allContinents.insert({continentName, continent});
                    }
                }
                Game::setAllContinents(allContinents);
            }

            if (line.find(TERRITORIES_TITLE) != string::npos) {
                while (getline(inFIle, line)) {
                    if (line.length() == 0) {
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
                        string continentName = countryTokens.at(CONTINENT_NAME_INDEX);
                        int numOfArmy = stoi(countryTokens.at(ARYM_NUMBER_INDEX));
                        Game::getAllContinents().find(continentName)->second.addCountryName(countryName);

                        vector<string> adjacentCountries;
                        for (int i = ADJACENT_COUNTRIES_STARTS; i < countryTokens.size(); i++) {
                            adjacentCountries.push_back(countryTokens.at(i));
                        }
                        Country country(countryName, coordinateX, coordinateY, adjacentCountries, numOfArmy);
                        country.setContinentName(continentName);
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
    for (auto &itr : Game::getAllCountries()) {
        int coorX = itr.second.getX();
        int coorY = itr.second.getY();
        int widthDia = COUNTRY_MARK_WIDTH / 2;
        int heightDia = COUNTRY_MARK_HEIGHT / 2;
        if (x <= coorX + widthDia && x >= coorX - widthDia && y >= coorY - heightDia && y <= coorY + heightDia) {
            return itr.first;
        }
    }
    return "";
}

void MapManager::renderCountryMark(int x, int y, Country &country, const int fontSize) {
    int ownerIndex = country.getOwnerIndex();
    Player player = Game::getAllPlayers().at(ownerIndex);

    renderMessage(x, y - COUNTRY_TEXT_HEIGHT_SHIFT, player.getPlayerName().c_str(), country.getTextColor(),
                  fontSize);
    renderMessage(x, y, country.getCountryName().c_str(), country.getTextColor(), fontSize);
    renderMessage(x, y + COUNTRY_TEXT_HEIGHT_SHIFT, to_string(country.getCountryArmy()).c_str(),
                  country.getTextColor(),
                  fontSize);
}


void MapManager::updateTextViewPort(vector<string> &messages, tuple<int, int, int, int> color) {
    SDL_Texture *textBgTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                   COUNTRY_INFO_WIDTH, COUNTRY_INFO_HEIGHT);
    SDL_SetRenderTarget(gRenderer, textBgTexture);

    SDL_RenderClear(gRenderer);

    int y = COUNTRY_INFO_START_Y;
    for (string &msg: messages) {
        renderMessage(COUNTRY_INFO_CENTER_X, y, msg.c_str(), color, COUNTRY_INFO_FONT_SIZE, DEFAULT_TEXT_FONT_PATH);
        y += COUNTRY_INFO_GAP;
    }

    SDL_SetRenderTarget(gRenderer, NULL);

    SDL_RenderCopy(gRenderer, textBgTexture, NULL, &countryInfoRect);

    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(textBgTexture);
}

void MapManager::updateWorldMap() {
    //SDL_RenderSetViewport(gRenderer,&worldMapRect);
    SDL_Texture *mapViewTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                    MAP_VIEW_PORT_WIDTH, MAP_VIEW_PORT_HEIGHT);
    SDL_SetRenderTarget(gRenderer, mapViewTexture);

    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gMapTexture, NULL, &worldMapRect);

    map<string, Country> allCountries = Game::getAllCountries();
    for (auto &item : allCountries) {
        Country &country = item.second;
        int x = country.getX();
        int y = country.getY();

        setOwnerColorMark(x, y, country.getCountryColour());

        renderCountryMark(x, y, country, DEFAULT_MAP_FONT_SIZE);
    }

    SDL_SetRenderTarget(gRenderer, NULL);
    //SDL_RenderSetClipRect(gRenderer, &worldMapRect);
    SDL_RenderCopy(gRenderer, mapViewTexture, NULL, &worldMapRect);


    SDL_RenderPresent(gRenderer);
    //SDL_DestroyTexture(mapViewTexture);

    //renderPlayerInfo();
}

void MapManager::resetToDefaultColor() {
    SDL_SetRenderDrawColor(gRenderer, get<0>(DEFAULT_BACKGROUND_COLOR), get<1>(DEFAULT_BACKGROUND_COLOR),
                           get<2>(DEFAULT_BACKGROUND_COLOR),
                           get<3>(DEFAULT_BACKGROUND_COLOR));
}

void MapManager::renderPlayerInfo() {
    SDL_Texture *playerTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                   COUNTRY_INFO_WIDTH, PLAYER_INFO_HEIGHT);
    SDL_SetRenderTarget(gRenderer, playerTexture);
    SDL_RenderClear(gRenderer);
    vector<Player> players = Game::getAllPlayers();
    int startY = PLAYER_INFO_Y;
    for (Player &player: players) {
        SDL_Rect rect{PLAYER_INFO_X, startY - PLAYER_INFO_RECT_WIDTH / 2, PLAYER_INFO_RECT_WIDTH,
                      PLAYER_INFO_RECT_WIDTH};
        SDL_SetRenderDrawColor(gRenderer, get<0>(player.getBgColor()), get<1>(player.getBgColor()),
                               get<2>(player.getBgColor()), get<3>(player.getBgColor()));
        SDL_RenderFillRect(gRenderer, &rect);

        auto allCountries = Game::getAllCountries();

        int counter = 0;

        for (auto &item: Game::getAllCountries()) {
            if (item.second.getOwnerIndex() == player.getPlayerIndex()) {
                counter++;
            }
        }

        string playerInfoStr = player.getPlayerName() + "  <" + to_string(counter) + ">";

        renderMessage(PLAYER_INFO_X + PLAYER_INFO_SPACE, startY, playerInfoStr.c_str(), player.getTextColor(),
                      PLAYER_INFO_FONT_SIZE, DEFAULT_TEXT_FONT_PATH);
        startY += PLAYER_INFO_GAP;
    }

    SDL_SetRenderTarget(gRenderer, NULL);
    SDL_RenderCopy(gRenderer, playerTexture, NULL, &playerInfoRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(playerTexture);
}

void MapManager::clearCountryInfo() {
    SDL_Texture *textBgTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                   COUNTRY_INFO_WIDTH, COUNTRY_INFO_HEIGHT);
    SDL_SetRenderTarget(gRenderer, textBgTexture);

    SDL_RenderClear(gRenderer);

    SDL_SetRenderTarget(gRenderer, NULL);

    SDL_RenderCopy(gRenderer, textBgTexture, NULL, &countryInfoRect);

    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(textBgTexture);
}

void MapManager::rednerNumberList(bool isDeployArmy, int numOfArmy) {
    SDL_Texture *numberTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                   NUMBER_LIST_WIDTH, NUMBER_LIST_HEIGHT);
    SDL_SetRenderTarget(gRenderer, numberTexture);
    SDL_RenderClear(gRenderer);

    int startX = NUMBER_LIST_X;
    numberMarkCoordinates.clear();

    if(isDeployArmy){

    }

    for (string numStr : NUMBER_STRING_VECTOR) {
        SDL_Rect rect{startX, NUMBER_LIST_Y, NUMBER_LIST_MARK_WIDTH, NUMBER_LIST_MARK_WIDTH};
        SDL_SetRenderDrawColor(gRenderer, get<0>(NUMBER_BACKGROUND_COLOR), get<1>(NUMBER_BACKGROUND_COLOR),
                               get<2>(NUMBER_BACKGROUND_COLOR), get<3>(NUMBER_BACKGROUND_COLOR));
        SDL_RenderFillRect(gRenderer, &rect);

        int centerX = startX + NUMBER_LIST_MARK_WIDTH / 2;
        int centerY = NUMBER_LIST_Y + NUMBER_LIST_MARK_WIDTH / 2;

        renderMessage(centerX, centerY, numStr.c_str(),
                      NUMBER_TEXT_COLOR, NUMBER_LIST_FONT_SIZE);

        numberMarkCoordinates.push_back({centerX + MAP_VIEW_PORT_WIDTH, centerY + PLAYER_INFO_HEIGHT + COUNTRY_INFO_HEIGHT});

        startX += NUMBER_LIST_SPACE;
    }

    SDL_SetRenderTarget(gRenderer, NULL);
    SDL_RenderCopy(gRenderer, numberTexture, NULL, &numberListRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(numberTexture);
    resetToDefaultColor();
}

bool MapManager::isDragFromNumber(SDL_Point point) {
    for (SDL_Point &p: numberMarkCoordinates) {
        if (point.x <= p.x + NUMBER_LIST_MARK_WIDTH / 2 && point.x >= p.x - NUMBER_LIST_MARK_WIDTH / 2
            && point.y >= p.y - NUMBER_LIST_MARK_WIDTH / 2 && point.y <= p.y + NUMBER_LIST_MARK_WIDTH / 2){
            return true;
        }
    }
    return false;
}

bool MapManager::isDragToOwnCountry(SDL_Point point, int playerIndex) {
    string countryName = getCountryNameFromCoordinates(point.x, point.y);
    if(countryName.empty()){
        return false;
    }
    Country& country = Game::getAllCountries().at(countryName);
    if(country.getOwnerIndex() != playerIndex){
        return false;
    }
    return true;
}

void MapManager::updateWholeScreen() {
    renderPlayerInfo();
    updateWorldMap();
}


