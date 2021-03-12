cmake_minimum_required(VERSION 3.17)
project(FinalGameDesign)

add_executable(FinalGameDesign main.cpp)


set(SDL2_LIB_DIR sdl2_lib/lib)
set(SDL2_INCLUDE_DIR sdl2_lib/include/SDL2)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

target_link_libraries(FinalGameDesign  -lmingw32 -lSDL2main -lSDL2)

