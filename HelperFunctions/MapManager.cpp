#include "MapManager.h"
#include "ColorList.h"

SDL_Window *MapManager::gWindow = nullptr;
SDL_Renderer *MapManager::gRenderer = nullptr;
SDL_Texture *MapManager::gMapTexture = nullptr;
TTF_Font *MapManager::gFont = nullptr;

const string MapManager::TERRITORIES_TITLE = "Territories";
const string MapManager::CONTINENT_TITLE = "Continents";
const string MapManager::PLAYER_TITLE = "Players";

const char *MapManager::DEFAULT_FONT_PATH = "../Fonts/FiraSans-Regular.ttf";
const char *MapManager::DEFAULT_TEXT_FONT_PATH = "../Fonts/NotoSansTC-Bold.otf";
const char *MapManager::BUTTONS_FONT_PATH = "../Fonts/FredokaOne-Regular.ttf";

vector<const char *> MapManager::CARD_IMAGE_PATH_LIST = {"../Images/Artillery.jpg", "../Images/Cavalry.jpg",
                                                         "../Images/Infantry.jpg"};
vector<string> MapManager::CARDS_TYPE_LIST{"Artillery", "Cavalry", "Infantry"};

const tuple<int, int, int, int> MapManager::DEFAULT_BACKGROUND_COLOR = ColorList::WHITE;
vector<string> MapManager::NUMBER_STRING_VECTOR{"1", "2", "5", "10", "ALL"};
const tuple<int, int, int, int> MapManager::NUMBER_BACKGROUND_COLOR{ColorList::BLACK};
const tuple<int, int, int, int> MapManager::NUMBER_TEXT_COLOR{ColorList::WHITE};
tuple<int, int, int, int> MapManager::CARDS_IMAGE_BORDER_COLOR{ColorList::BLACK};
const tuple<int, int, int, int> MapManager::BUTTONS_BACKGROUND_COLOR{ColorList::LIGHTER_YELLOW};
const tuple<int, int, int, int> MapManager::BUTTONS_BORDER_COLOR{ColorList::BLACK};
tuple<int, int, int, int> MapManager::BUTTONS_TEXT_COLOR{ColorList::BLACK};

vector<string> MapManager::buttonNames{"LOAD", "SAVE", "RESET", "NEXT"};

Country *MapManager::fromCountry{};
Country *MapManager::toCountry{};

double MapManager::IMAGE_WIDTH_RATIO = 1.0;
double MapManager::IMAGE_HEIGHT_RATIO = 1.0;

const SDL_Rect MapManager::countryInfoRect{MAP_VIEW_PORT_WIDTH, PLAYER_INFO_HEIGHT, COUNTRY_INFO_WIDTH,
                                     COUNTRY_INFO_HEIGHT};
const SDL_Rect MapManager::worldMapRect{0, 0, MAP_VIEW_PORT_WIDTH, MAP_VIEW_PORT_HEIGHT};
const SDL_Rect MapManager::playerInfoRect{MAP_VIEW_PORT_WIDTH, 0, PLAYER_INFO_WIDTH, PLAYER_INFO_HEIGHT};
const SDL_Rect MapManager::numberListRect{MAP_VIEW_PORT_WIDTH, NUMBER_LIST_ABSOLUTE_Y, NUMBER_LIST_WIDTH, NUMBER_LIST_HEIGHT};
const SDL_Rect MapManager::cardsListRect{MAP_VIEW_PORT_WIDTH, CARDS_LIST_ABSOLUTE_Y, CARDS_LIST_WIDTH,
                                   CARDS_LIST_HEIGHT};
const SDL_Rect MapManager::buttonsRect{MAP_VIEW_PORT_WIDTH, BUTTONS_ABSOLUTE_Y, BUTTONS_WIDTH, BUTTONS_HEIGHT};

vector<SDL_Point> MapManager::numberMarkCoordinates{vector<SDL_Point>()};
map<ButtonType, SDL_Point> MapManager::buttonCoordinates{map<ButtonType, SDL_Point>()};

const SDL_MessageBoxButtonData MESSAGE_BOX_BUTTONS[2] = {
        {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "No"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes"},
};

const SDL_MessageBoxColorScheme MapManager::MESSAGE_BOX_COLOR_SCHEME{
        { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                {255, 0, 0},
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {0, 255, 0},
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                {255, 255, 0},
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {0, 0, 255},
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                {255, 0, 255}
        }
};

void MapManager::initCountryMarks() {
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
        if (gWindow == nullptr) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr) {
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
    if (gMapTexture == nullptr) {
        printf("Failed to load texture image!\n");
        success = false;
    }
    return success;
}

void MapManager::SDLClose() {
    //Free loaded image
    SDL_DestroyTexture(gMapTexture);
    gMapTexture = nullptr;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    //Free global font
    TTF_CloseFont(gFont);
    gFont = nullptr;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *MapManager::loadTexture(string path) {
    //The final texture
    SDL_Texture *newTexture = nullptr;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr) {
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
            initMapImage();

            Game::readMapConfigFromFile();
            Game::initPlayersAndCountries();

            initCountryMarks();

            Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
            curPlayer.getCalUndeployArmyNumber();
            //updateWholeScreen();

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
                    stringstream ss;
                    SDL_Point dragStartPoint, dragEndPoint;
                    bool countriesDraggedForMove;

                    Player &currentPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());

                    int buttonId;
                    const SDL_MessageBoxData resetMsgBoxData = {
                            SDL_MESSAGEBOX_INFORMATION, /* .flags */
                            nullptr, /* .window */
                            "Reset Game", /* .title */
                            "Danger: you are resetting the game!", /* .message */
                            SDL_arraysize(MESSAGE_BOX_BUTTONS), /* .numbuttons */
                            MESSAGE_BOX_BUTTONS, /* .buttons */
                            &MESSAGE_BOX_COLOR_SCHEME /* .colorScheme */
                    };

                    switch (e.type) {
                        case SDL_QUIT:
                            quit = true;
                            break;

                        case SDL_MOUSEMOTION:
                            message = to_string(e.motion.x) + ":" + to_string(e.motion.y);
                            if (!getCountryNameFromCoordinates(e.motion.x, e.motion.y).empty()) {
                                message = getCountryNameFromCoordinates(e.motion.x, e.motion.y);
                                clickedCountry = &Game::getAllCountries().find(message)->second;
                                renderCountryInfoRect(clickedCountry);
                            } else {
                                //clickedCountry = nullptr;
                                clearCountryInfoRect();
                            }
                            SDL_SetWindowTitle(gWindow, message.c_str());
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            switch (e.button.button) {
                                case SDL_BUTTON_LEFT:
                                    dragStartPoint = {e.motion.x, e.motion.y};

                                    //Move army if in Move stage and click a number button
                                    int numberIndex = getIndexOfDraggedNumber(dragStartPoint);
                                    if (numberIndex >= 0 && Game::getGameStage() == MOVE && countriesDraggedForMove) {
                                        int number;
                                        if (numberIndex == NUMBER_STRING_VECTOR.size() - 1) {
                                            number = fromCountry->getNumOfArmy() - 1;
                                        } else {
                                            number = stoi(NUMBER_STRING_VECTOR.at(numberIndex));
                                        }
                                        fromCountry->reduceNumOfArmy(number);
                                        toCountry->addNumOfArmy(number);
                                        countriesDraggedForMove = false;

                                        //CHECK HERE
                                        nextStage();

                                    } else {
                                        ButtonType buttonType = clickButtonType(dragStartPoint);
                                        if (buttonType != NONE) {
                                            switch (buttonType) {
                                                case LOAD:
                                                    //FIXEME
                                                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                                                             "LOAD",
                                                                             "Click LOAD", nullptr);

                                                    loadGameFromFile();
                                                    break;
                                                case SAVE:
                                                    //FIXME
                                                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                                                             "SAVE",
                                                                             "Click SAVE", nullptr);
                                                    saveGameToFile();
                                                    break;
                                                case RESET:
                                                    if (SDL_ShowMessageBox(&resetMsgBoxData, &buttonId) < 0) {
                                                        SDL_Log("error displaying message box");
                                                    }
                                                    if (buttonId == 1) {
                                                        resetGame();
                                                    }
                                                    break;
                                                case NEXT:
                                                    nextStage();
                                                    break;
                                                default:
                                                    break;
                                            }
                                        }
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
                                        fromCountryName != toCountryName && isAdjacentCountry(fromCountry, toCountry)
                                        && currentPlayer.getPlayerIndex() == fromCountry->getOwnerIndex()) {
                                        if (isSameOwner(fromCountryName, toCountryName) &&
                                            Game::getGameStage() == ATTACK) {
                                            ss.str("");
                                            ss.clear();
                                            ss << "[" << fromCountryName << "] and [" << toCountryName
                                               << "] are both your countries!";
                                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                                                     "Forbidden",
                                                                     ss.str().c_str(), nullptr);
                                        } else {
                                            //Attack stage if drag&drop on two countries from different players
                                            if (Game::getGameStage() == ATTACK) {
                                                if (fromCountry->getNumOfArmy() > 1) {
                                                    ss.str("");
                                                    ss.clear();
                                                    ss << currentPlayer.getPlayerName() << " ATTACKS [" << toCountryName
                                                       << "] FROM [" << fromCountryName << "].";

                                                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                                                             "Attack Country",
                                                                             ss.str().c_str(), nullptr);
                                                    Game::attackFrom(*fromCountry, *toCountry);

                                                    //check whether the player has any country for attacking
                                                    if (!canAttackFromAnyCountry(currentPlayer)) {
                                                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                                                                 "Next: Move",
                                                                                 "No any country for further attacking! Next player",
                                                                                 nullptr);

                                                        nextStage();
                                                        nextStage();
                                                    }
                                                    updateWholeScreen();
                                                } else {
                                                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                                                             "Attack Country",
                                                                             "No enough army (>1) for attacking!",
                                                                             nullptr);
                                                }
                                            }
                                            //Move stage if drag&Drop on two countries from the same player.
                                            if (Game::getGameStage() == MOVE &&
                                                isDragToOwnCountry(dragEndPoint, currentPlayer.getPlayerIndex())) {
                                                countriesDraggedForMove = true;
                                                updateWholeScreen();
                                            }
                                        }
                                    }

                                    //deploy numbers if drag from number rect to an own country
                                    int numberIndex = getIndexOfDraggedNumber(dragStartPoint);
                                    if (numberIndex >= 0) {
                                        if (isDragToOwnCountry(dragEndPoint, currentPlayer.getPlayerIndex())) {
                                            string countryName = getCountryNameFromCoordinates(dragEndPoint.x,
                                                                                               dragEndPoint.y);
                                            if (Game::getGameStage() == DEPLOYMENT) {
                                                Country &deployCountry = Game::getAllCountries().at(countryName);
                                                int number;
                                                if (numberIndex == NUMBER_STRING_VECTOR.size() - 1) {
                                                    number = currentPlayer.getUndeployArmyNumber();
                                                } else {
                                                    number = stoi(NUMBER_STRING_VECTOR.at(numberIndex));
                                                }

                                                currentPlayer.removeUndeployArmy(number);
                                                deployCountry.addNumOfArmy(number);

                                                //Message box body, show num of army deployed
                                                ss.str("");
                                                ss.clear();

                                                ss << currentPlayer.getPlayerName() << " deployed <";

                                                if (currentPlayer.getUndeployArmyNumber() == 0) {
                                                    Game::setGameStage(ATTACK);
                                                    ss << "ALL";
                                                    Game::ifClickedNext = false;
                                                } else if (Game::isHumanPlayer) {
                                                    ss << number;
                                                }
                                                ss << "> armies to [" << countryName << "].";

                                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                                                         "Deploy Army",
                                                                         ss.str().c_str(), nullptr);

                                                updateWholeScreen();
                                            }
                                        }
                                    }
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

void MapManager::renderCountryInfoRect(Country *pickedCountry) {
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
    updateCountryInfoRect(messages, player.getTextColor());
}

void MapManager::initMapImage() {
    //SDL_RenderSetViewport(gRenderer, &worldMapRect);
    SDL_RenderCopy(gRenderer, gMapTexture, nullptr, &worldMapRect);
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
    renderPlayerInfoRect();
    resetToDefaultColor();
    renderNumberListRect();
    renderCardsListRect();
    renderButtonsRect();
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

    SDL_RenderCopy(gRenderer, textTexture, nullptr, &dest);

    // SDL_SetRenderTarget(gRenderer, NULL);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    resetToDefaultColor();
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


void MapManager::updateCountryInfoRect(vector<string> &messages, tuple<int, int, int, int> color) {
    SDL_Texture *textBgTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                   COUNTRY_INFO_WIDTH, COUNTRY_INFO_HEIGHT);
    SDL_SetRenderTarget(gRenderer, textBgTexture);
    SDL_RenderClear(gRenderer);

    int y = COUNTRY_INFO_START_Y;
    for (string &msg: messages) {
        renderMessage(COUNTRY_INFO_CENTER_X, y, msg.c_str(), color, COUNTRY_INFO_FONT_SIZE, DEFAULT_TEXT_FONT_PATH);
        y += COUNTRY_INFO_GAP;
    }

    SDL_SetRenderTarget(gRenderer, nullptr);
    SDL_RenderCopy(gRenderer, textBgTexture, nullptr, &countryInfoRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(textBgTexture);
}

void MapManager::updateMapRect() {
    SDL_Texture *mapViewTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                    MAP_VIEW_PORT_WIDTH, MAP_VIEW_PORT_HEIGHT);
    SDL_SetRenderTarget(gRenderer, mapViewTexture);
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, gMapTexture, nullptr, &worldMapRect);

    map<string, Country> allCountries = Game::getAllCountries();
    for (auto &item : allCountries) {
        Country &country = item.second;
        int x = country.getX();
        int y = country.getY();

        setOwnerColorMark(x, y, country.getCountryColour());
        renderCountryMark(x, y, country, DEFAULT_MAP_FONT_SIZE);
    }

    SDL_SetRenderTarget(gRenderer, nullptr);
    SDL_RenderCopy(gRenderer, mapViewTexture, nullptr, &worldMapRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(mapViewTexture);
}

void MapManager::resetToDefaultColor() {
    SDL_SetRenderDrawColor(gRenderer, get<0>(DEFAULT_BACKGROUND_COLOR), get<1>(DEFAULT_BACKGROUND_COLOR),
                           get<2>(DEFAULT_BACKGROUND_COLOR),
                           get<3>(DEFAULT_BACKGROUND_COLOR));
}

void MapManager::renderPlayerInfoRect() {
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

    SDL_SetRenderTarget(gRenderer, nullptr);
    SDL_RenderCopy(gRenderer, playerTexture, nullptr, &playerInfoRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(playerTexture);
}

void MapManager::clearCountryInfoRect() {
    SDL_Texture *textBgTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                   COUNTRY_INFO_WIDTH, COUNTRY_INFO_HEIGHT);
    SDL_SetRenderTarget(gRenderer, textBgTexture);
    SDL_RenderClear(gRenderer);

    SDL_SetRenderTarget(gRenderer, nullptr);
    SDL_RenderCopy(gRenderer, textBgTexture, nullptr, &countryInfoRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(textBgTexture);
}

void MapManager::renderNumberListRect() {
    SDL_Texture *numberTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                   NUMBER_LIST_WIDTH, NUMBER_LIST_HEIGHT);
    SDL_SetRenderTarget(gRenderer, numberTexture);
    SDL_RenderClear(gRenderer);

    int startX = NUMBER_LIST_X;
    int startY = NUMBER_LIST_HEIGHT / 4;
    numberMarkCoordinates.clear();
    Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
    stringstream ss;
    int numOfArmy = 0;

    renderMessage(NUMBER_LIST_WIDTH / 2, startY, curPlayer.getPlayerName().c_str(), curPlayer.getTextColor(),
                  NUMBER_LIST_FONT_SIZE);
    startY += NUMBER_LIST_GAP;

    if (Game::getGameStage() == GameStage::DEPLOYMENT) {
        ss << "Undeploy Army:   " << curPlayer.getUndeployArmyNumber();
        renderMessage(NUMBER_LIST_WIDTH / 2, startY, ss.str().c_str(), curPlayer.getTextColor(),
                      NUMBER_LIST_UNDEPLOY_FONT_SIZE);
        numOfArmy = curPlayer.getUndeployArmyNumber();
    }

    if (Game::getGameStage() == GameStage::MOVE) {
        ss << "Stage: Move Army";
        renderMessage(NUMBER_LIST_WIDTH / 2, startY, ss.str().c_str(), curPlayer.getTextColor(),
                      NUMBER_LIST_UNDEPLOY_FONT_SIZE);
        if (fromCountry != nullptr) {
            numOfArmy = fromCountry->getNumOfArmy() - 1;
        }
    }

    if (Game::getGameStage() == GameStage::ATTACK) {
        ss << "Stage: Attack Country";
        renderMessage(NUMBER_LIST_WIDTH / 2, startY, ss.str().c_str(), curPlayer.getTextColor(),
                      NUMBER_LIST_UNDEPLOY_FONT_SIZE);
    }

    if (Game::getGameStage() == GameStage::EXCHANGE_CARDS) {
        ss << "Stage: Exchange-Cards";
        renderMessage(NUMBER_LIST_WIDTH / 2, startY, ss.str().c_str(), curPlayer.getTextColor(),
                      NUMBER_LIST_UNDEPLOY_FONT_SIZE);
    }

    startY += NUMBER_LIST_GAP;

    for (int i = 0; i < NUMBER_STRING_VECTOR.size(); i++) {
        string numStr = NUMBER_STRING_VECTOR.at(i);
        SDL_Rect rect{startX, startY, NUMBER_LIST_MARK_WIDTH, NUMBER_LIST_MARK_WIDTH};
        SDL_SetRenderDrawColor(gRenderer, get<0>(NUMBER_BACKGROUND_COLOR), get<1>(NUMBER_BACKGROUND_COLOR),
                               get<2>(NUMBER_BACKGROUND_COLOR), get<3>(NUMBER_BACKGROUND_COLOR));
        SDL_RenderFillRect(gRenderer, &rect);

        int centerX = startX + NUMBER_LIST_MARK_WIDTH / 2;
        int centerY = startY + NUMBER_LIST_MARK_WIDTH / 2;

        if ((Game::getGameStage() == DEPLOYMENT ||
             (Game::getGameStage() == MOVE && fromCountry != nullptr
              && toCountry != nullptr)) && (numOfArmy >= 1) &&
            (i == NUMBER_STRING_VECTOR.size() - 1 || stoi(numStr) <= numOfArmy)) {
            renderMessage(centerX, centerY, numStr.c_str(),
                          curPlayer.getBgColor(), NUMBER_LIST_FONT_SIZE);
        } else {
            renderMessage(centerX, centerY, numStr.c_str(),
                          NUMBER_TEXT_COLOR, NUMBER_LIST_FONT_SIZE);
        }

        numberMarkCoordinates.push_back(
                {centerX + MAP_VIEW_PORT_WIDTH, centerY + PLAYER_INFO_HEIGHT + COUNTRY_INFO_HEIGHT});

        startX += NUMBER_LIST_SPACE;
    }

    SDL_SetRenderTarget(gRenderer, nullptr);
    SDL_RenderCopy(gRenderer, numberTexture, nullptr, &numberListRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(numberTexture);
    resetToDefaultColor();
}

int MapManager::getIndexOfDraggedNumber(SDL_Point point) {
    for (int i = 0; i < numberMarkCoordinates.size(); i++) {
        SDL_Point &p = numberMarkCoordinates.at(i);
        if (point.x <= p.x + NUMBER_LIST_MARK_WIDTH / 2 && point.x >= p.x - NUMBER_LIST_MARK_WIDTH / 2
            && point.y >= p.y - NUMBER_LIST_MARK_WIDTH / 2 && point.y <= p.y + NUMBER_LIST_MARK_WIDTH / 2) {
            return i;
        }
    }
    return -1;
}

bool MapManager::isDragToOwnCountry(SDL_Point point, int playerIndex) {
    string countryName = getCountryNameFromCoordinates(point.x, point.y);
    if (countryName.empty()) {
        return false;
    }
    Country &country = Game::getAllCountries().at(countryName);
    if (country.getOwnerIndex() != playerIndex) {
        return false;
    }
    return true;
}

void MapManager::updateWholeScreen() {
    renderPlayerInfoRect();
    updateMapRect();
    renderNumberListRect();
    renderCardsListRect();
    renderButtonsRect();
}

void MapManager::renderCardsListRect() {
    SDL_Texture *cardsTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                  CARDS_LIST_WIDTH, CARDS_LIST_HEIGHT);
    SDL_SetRenderTarget(gRenderer, cardsTexture);
    SDL_RenderClear(gRenderer);

    int startX = CARDS_LIST_X;
    int startY = CARDS_LIST_Y;

    Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());

    SDL_Surface *cardImageSurface;
    SDL_Texture *cardImageTexture;

    //type of card is 3
    for (int i = 0; i < CARD_IMAGE_PATH_LIST.size(); i++) {
        cardImageSurface = IMG_Load(CARD_IMAGE_PATH_LIST.at(i));
        if (cardImageSurface != nullptr) {
            cardImageTexture = SDL_CreateTextureFromSurface(gRenderer, cardImageSurface);
            SDL_Rect cardRect{startX, startY, CARDS_LIST_IMAGE_WIDTH, CARDS_LIST_IMAGE_HEIGHT};
            SDL_Rect cardBorderRect{startX - 2, startY - 2, CARDS_LIST_IMAGE_WIDTH + 4, CARDS_LIST_IMAGE_HEIGHT + 4};
            SDL_SetRenderDrawColor(gRenderer, get<0>(CARDS_IMAGE_BORDER_COLOR), get<1>(CARDS_IMAGE_BORDER_COLOR),
                                   get<2>(CARDS_IMAGE_BORDER_COLOR), get<3>(CARDS_IMAGE_BORDER_COLOR));
            SDL_RenderDrawRect(gRenderer, &cardBorderRect);
            SDL_RenderCopy(gRenderer, cardImageTexture, nullptr, &cardRect);

            renderMessage(startX + CARDS_LIST_SPACE, startY + CARDS_LIST_IMAGE_HEIGHT / 2 - CARDS_TEXT_GAP / 2,
                          CARDS_TYPE_LIST.at(i).c_str(),
                          curPlayer.getTextColor(), CARDS_LIST_FONT_SIZE);

            CardType cardType = getCardTypeFromString(CARDS_TYPE_LIST.at(i));
            int numOfCards = curPlayer.getCardNumOfType(cardType);
            renderMessage(startX + CARDS_LIST_SPACE, startY + CARDS_LIST_IMAGE_HEIGHT / 2 + CARDS_TEXT_GAP / 2,
                          to_string(numOfCards).c_str(), curPlayer.getTextColor(), CARDS_LIST_FONT_SIZE);
            startY += CARDS_LIST_IMAGE_HEIGHT + CARDS_LIST_GAP;
        }
    }

    SDL_SetRenderTarget(gRenderer, nullptr);
    SDL_RenderCopy(gRenderer, cardsTexture, nullptr, &cardsListRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(cardsTexture);
    resetToDefaultColor();
}

void MapManager::renderButtonsRect() {
    SDL_Texture *buttonsTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                                    COUNTRY_INFO_WIDTH, COUNTRY_INFO_HEIGHT);
    SDL_SetRenderTarget(gRenderer, buttonsTexture);
    SDL_RenderClear(gRenderer);

    int startX = BUTTONS_LEFT_X;
    int startY = BUTTONS_UPPER_Y;
    buttonCoordinates.clear();

    //two lines of MESSAGE_BOX_BUTTONS
    for (int i = 0; i < 2; i++) {
        SDL_Rect buttonRect1{startX, startY, BUTTONS_RECT_WIDTH, BUTTONS_RECT_HEIGHT};
        SDL_SetRenderDrawColor(gRenderer, get<0>(BUTTONS_BACKGROUND_COLOR), get<1>(BUTTONS_BACKGROUND_COLOR),
                               get<2>(BUTTONS_BACKGROUND_COLOR), get<3>(BUTTONS_BACKGROUND_COLOR));
        SDL_RenderFillRect(gRenderer, &buttonRect1);
        SDL_SetRenderDrawColor(gRenderer, get<0>(BUTTONS_BORDER_COLOR), get<1>(BUTTONS_BORDER_COLOR),
                               get<2>(BUTTONS_BORDER_COLOR), get<3>(BUTTONS_BORDER_COLOR));
        SDL_RenderDrawRect(gRenderer, &buttonRect1);
        string buttonName1 = buttonNames.at(i * 2);
        renderMessage(startX + BUTTONS_RECT_WIDTH / 2, startY + BUTTONS_RECT_HEIGHT / 2, buttonName1.c_str(),
                      BUTTONS_TEXT_COLOR, BUTTONS_TEXT_FONT_SIZE, BUTTONS_FONT_PATH);
        SDL_Point buttonPoint1{MAP_VIEW_PORT_WIDTH + startX, BUTTONS_ABSOLUTE_Y + startY};
        buttonCoordinates.insert({getButtonTypeFromStr(buttonName1), buttonPoint1});

        startX += BUTTONS_RECT_WIDTH + BUTTONS_SPACE;

        SDL_Rect buttonRect2{startX, startY, BUTTONS_RECT_WIDTH, BUTTONS_RECT_HEIGHT};
        SDL_SetRenderDrawColor(gRenderer, get<0>(BUTTONS_BACKGROUND_COLOR), get<1>(BUTTONS_BACKGROUND_COLOR),
                               get<2>(BUTTONS_BACKGROUND_COLOR), get<3>(BUTTONS_BACKGROUND_COLOR));
        SDL_RenderFillRect(gRenderer, &buttonRect2);
        SDL_SetRenderDrawColor(gRenderer, get<0>(BUTTONS_BORDER_COLOR), get<1>(BUTTONS_BORDER_COLOR),
                               get<2>(BUTTONS_BORDER_COLOR), get<3>(BUTTONS_BORDER_COLOR));
        SDL_RenderDrawRect(gRenderer, &buttonRect2);
        string buttonName2 = buttonNames.at(i * 2 + 1);
        renderMessage(startX + BUTTONS_RECT_WIDTH / 2, startY + BUTTONS_RECT_HEIGHT / 2,
                      buttonName2.c_str(),
                      BUTTONS_TEXT_COLOR, BUTTONS_TEXT_FONT_SIZE, BUTTONS_FONT_PATH);

        SDL_Point buttonPoint2{MAP_VIEW_PORT_WIDTH + startX, BUTTONS_ABSOLUTE_Y + startY};
        buttonCoordinates.insert({getButtonTypeFromStr(buttonName2), buttonPoint2});

        startX = BUTTONS_LEFT_X;
        startY += BUTTONS_RECT_HEIGHT + BUTTONS_GAP;
    }

    SDL_SetRenderTarget(gRenderer, nullptr);
    SDL_RenderCopy(gRenderer, buttonsTexture, nullptr, &buttonsRect);
    SDL_RenderPresent(gRenderer);
    SDL_DestroyTexture(buttonsTexture);
    resetToDefaultColor();
}

//FIXME
void MapManager::loadGameFromFile() {

}

ButtonType MapManager::clickButtonType(SDL_Point &p) {
    for (auto &item: buttonCoordinates) {
        SDL_Point &point = item.second;
        if (p.x >= point.x && p.x <= point.x + BUTTONS_RECT_WIDTH && p.y >= point.y &&
            p.y <= point.y + BUTTONS_RECT_HEIGHT) {
            return item.first;
        }
    }
    return NONE;
}

//FIXME
void MapManager::saveGameToFile() {

}

void MapManager::resetGame() {
    Game::setCurPlayerIndex(0);
    Game::getAllPlayers().clear();
    Game::setGameStage(DEPLOYMENT);
    Game::getAllCountries().clear();
    Game::getAllContinents().clear();

    SDL_RenderClear(gRenderer);
    initMapImage();
    Game::readMapConfigFromFile();
    Game::initPlayersAndCountries();
    initCountryMarks();

    Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
    curPlayer.getCalUndeployArmyNumber();

    initTextViewPort();
    SDL_RenderPresent(gRenderer);
}


void MapManager::nextStage() {
    if (Game::isHumanPlayer) {
        if (Game::getGameStage() == ATTACK) {
            if (Game::isConquerACountry) {
                Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
                CardType card = Card::getBonusCard();
                curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
                curPlayer.addCard(card);
                Game::isConquerACountry = false;
            }
            Game::setGameStage(MOVE);
            fromCountry = nullptr;
            toCountry = nullptr;
        } else if (Game::getGameStage() == MOVE) {
            int curPlayerIndex = Game::getCurPlayerIndex();
            int playerIndex = ++curPlayerIndex % Game::getAllPlayers().size();
            Game::setCurPlayerIndex(playerIndex);
            while (Game::getPlayerCountries(Game::getCurPlayerIndex()).empty()) {
                if (playerIndex == 0) {
                    Game::isHumanPlayer = true;
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                             "You Lose",
                                             "You lose all countries!", nullptr);
                    break;
                }

                curPlayerIndex = Game::getCurPlayerIndex();
                playerIndex = ++curPlayerIndex % Game::getAllPlayers().size();
                Game::setCurPlayerIndex(playerIndex);
            }
            Game::setCurPlayerIndex(playerIndex);
            Game::setGameStage(DEPLOYMENT);
            Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
            curPlayer.getCalUndeployArmyNumber();
            fromCountry = nullptr;
            toCountry = nullptr;
            if (curPlayerIndex != 0) {
                Game::isHumanPlayer = false;

                //Robot plarying starts from here
                while (Game::getCurPlayerIndex() != 0) {
                    Player &robotPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
                    robotPlayer.getCalUndeployArmyNumber();

                    Game::robotDeploy();
                    updateWholeScreen();

                    Game::setGameStage(ATTACK);
                    updateWholeScreen();

                    Game::robotAttack();
                    if (Game::isConquerACountry) {
                        CardType card = Card::getBonusCard();
                        robotPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
                        robotPlayer.addCard(card);
                        Game::isConquerACountry = false;
                    }

                    updateWholeScreen();

                    Game::setGameStage(MOVE);
                    updateWholeScreen();

                    Game::robotMove();
                    updateWholeScreen();

                    int curPlayerIndex = Game::getCurPlayerIndex();
                    int playerIndex = ++curPlayerIndex % Game::getAllPlayers().size();
                    Game::setCurPlayerIndex(playerIndex);
                    while (Game::getPlayerCountries(Game::getCurPlayerIndex()).empty()) {
                        if (playerIndex == 0) {
                            Game::isHumanPlayer = true;
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                                     "You Lose",
                                                     "You lose all countries!", nullptr);
                            break;
                        }

                        curPlayerIndex = Game::getCurPlayerIndex();
                        playerIndex = ++curPlayerIndex % Game::getAllPlayers().size();
                        Game::setCurPlayerIndex(playerIndex);
                    }

                    Game::setCurPlayerIndex(playerIndex);
                    Game::setGameStage(DEPLOYMENT);
                    Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
                    curPlayer.getCalUndeployArmyNumber();
                    updateWholeScreen();
                }
            } else if (Game::getGameStage() == DEPLOYMENT) {
                Player &curPlayer = Game::getAllPlayers().at(Game::getCurPlayerIndex());
                int undeployArmy = curPlayer.getUndeployArmyNumber();
                if (undeployArmy > 0) {
                    if (Game::ifClickedNext) {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Deploy Stage",
                                                 "Please deploy armies before attacking!", nullptr);
                    } else {
                        Game::ifClickedNext = true;
                    }
                } else {
                    Game::setGameStage(ATTACK);
                    Game::ifClickedNext = false;
                }
            }
        } else {

        }
        Game::isHumanPlayer = true;
    }
    updateWholeScreen();
}

ButtonType MapManager::getButtonTypeFromStr(const string& buttonName) {
    if (buttonName == buttonNames.at(0)) {
        return LOAD;
    }
    if (buttonName == buttonNames.at(1)) {
        return SAVE;
    }
    if (buttonName == buttonNames.at(2)) {
        return RESET;
    }
    if (buttonName == buttonNames.at(3)) {
        return NEXT;
    }
    return NONE;
}

CardType MapManager::getCardTypeFromString(string &cardStr) {
    if (cardStr == "Artillery") {
        return ARTILLERY;
    }
    if (cardStr == "Cavalry") {
        return CAVALRY;
    }
    return INFANTRY;
}

bool MapManager::isAdjacentCountry(Country *attackingCountry, Country *defendingCountry) {
    for (const string &countryName: attackingCountry->getAdjacentCountires()) {
        if (countryName == defendingCountry->getCountryName()) {
            return true;
        }
    }
    if (attackingCountry->getOwnerIndex() == Game::getCurPlayerIndex() && Game::isHumanPlayer) {
        stringstream ss;
        ss << "[" << attackingCountry->getCountryName() << "] and [" << defendingCountry->getCountryName()
           << "] are NOT adjacent!";
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
                                 "Not Adjacent Countries",
                                 ss.str().c_str(), nullptr);
    }
    return false;
}

bool MapManager::canAttackFromAnyCountry(Player &player) {
    for (auto *country : Game::getPlayerCountries(player.getPlayerIndex())) {
        if (country->getNumOfArmy() > 1) {
            return true;
        }
    }
    return false;
}

bool MapManager::isSameOwner(const string& fromCountryName, const string& toCountryName) {
    return Game::getAllCountries().at(fromCountryName).getOwnerIndex() ==
           Game::getAllCountries().at(toCountryName).getOwnerIndex();
}

GameStage MapManager::getGameStageFromString(string stageStr){

    if (stageStr == "DEPLOYMENT"){
        return DEPLOYMENT;
    }
    if (stageStr == "ATTACK"){
        return ATTACK;
    }
    if (stageStr == "MOVE"){
    return MOVE;
    }
    if (stageStr == "EXCHANGE_CARDS"){
    return EXCHANGE_CARDS;
    }
}
