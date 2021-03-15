cmake_minimum_required(VERSION 3.17)
project(FinalGameDesign)

add_executable(FinalGameDesign main.cpp Archer.cpp Archer.h Army.h Army.cpp Soldier.cpp Soldier.h Continent.cpp Continent.h Country.cpp Country.h Horseman.cpp Horseman.h
        Swordsman.cpp Swordsman.h HelperFunctions/imports.h Player.cpp Player.h HelperFunctions/MapManager.cpp HelperFunctions/MapManager.h HelperFunctions/ColorList.h HelperFunctions/ColorList.cpp
        HelperFunctions/Game.h HelperFunctions/Game.cpp Random.h Random.cpp)


#[[add_executable(FinalGameDesign SDL2SampleCodes/17_mouse_events/17_mouse_events.cpp)]]


set(SDL2_LIB_DIR lib_sdl2/lib)
set(SDL2_INCLUDE_DIR lib_sdl2/include/SDL2)

include_directories(${SDL2_INCLUDE_DIR})

link_directories(${SDL2_LIB_DIR})

target_link_libraries(FinalGameDesign -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows)
