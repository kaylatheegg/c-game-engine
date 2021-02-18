cmake_minimum_required(VERSION 3.17)
project(Engine C)

set(CMAKE_C_STANDARD 11)

set(LCC gcc)
set(WCC x86_64-w64-mingw32-gcc-win32)

set(LCFLAGS m SDL2 SDL2_image SDL2_ttf)
set(DEBUGFLAGS "-g -rdynamic")
set(DONTBEAFUCKINGIDIOT "-Werror -Wall -pedantic -fsanitize=address")

# set(WCFLAGS -lmingw32 -lm -mwindows -Ilibs/ -Llibs/SDL2/ -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libgcc -static-libstdc++ -Werror)
# set(WSFLAGS, -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid)

set(CMAKE_C_FLAGS "${DEBUGFLAGS} ${DONTBEAFUCKINGIDIOT}")

file(GLOB_RECURSE SOURCES engine/src/*.c)

add_executable(Engine ${SOURCES})
target_link_libraries(Engine ${LCFLAGS})