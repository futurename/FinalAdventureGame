cmake_minimum_required(VERSION 3.17)
project(FinalGameDesign)

add_executable(FinalGameDesign main.cpp Continent.cpp Continent.h Country.cpp Country.h
        HelperFunctions/imports.h Player.cpp Player.h HelperFunctions/MapManager.cpp HelperFunctions/MapManager.h HelperFunctions/ColorList.h HelperFunctions/ColorList.cpp
        HelperFunctions/Game.h HelperFunctions/Game.cpp HelperFunctions/Random.h HelperFunctions/Random.cpp HelperFunctions/Card.h HelperFunctions/Card.cpp)

set(SDL2_LIB_DIR lib_sdl2/lib)
set(SDL2_INCLUDE_DIR lib_sdl2/include/SDL2)

include_directories(${SDL2_INCLUDE_DIR})

link_directories(${SDL2_LIB_DIR})

target_link_libraries(FinalGameDesign -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows)
