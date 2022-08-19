#finds all files in engine/source in all directories that have the .c suffix
SRCPATHS = engine/src/*/*.c

CSRC = $(wildcard $(SRCPATHS))

OBJECTS = $(CSRC:.c=.o)

#LCC is our linux compiler
LCC = gcc

#WCC is our windows compiler
WCC = x86_64-w64-mingw32-gcc-win32

#LCFLAGS are our compile time flags for linux
LCFLAGS = -lm -lSDL2 -lSDL2_image -lGL -lGLEW -lfreetype -Iengine/includes -I/usr/include/freetype2 -I/usr/include/libpng16

DEBUGFLAGS = -g -rdynamic -DDEBUG -O3

DONTBEAFUCKINGIDIOT = -Werror -Wall -Wextra -pedantic -Wno-missing-field-initializers

#WCFLAGS are our compile time flags for windows
WCFLAGS = -lmingw32 -lm -mwindows -Iengine/includes -Ilibs/ -Llibs/lib -static-libgcc -static-libstdc++ -Werror -lSDL2main -lSDL2 -lSDL2_image -lglew32 -lopengl32 -lfreetype

#i dont know what these do, they appear to "support" the compilation
WSFLAGS = -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid 

%.o: %.c
	$(LCC) -c -o $@ $< $(LCFLAGS) $(DEBUGFLAGS) $(DONTBEAFUCKINGIDIOT)

a.out : $(OBJECTS)
	$(LCC) $(OBJECTS) $(LCFLAGS) $(DEBUGFLAGS) $(DONTBEAFUCKINGIDIOT)

a.exe : $(CSRC)
	$(WCC) $(CSRC) $(WCFLAGS) $(WSFLAGS) -D__WIN32__ -DDEBUG 

documentation:
	doxygen


clean: 
	rm -f engine/src/*/*.o
	rm -rf html/
	rm -rf latex/
	rm -rf game/
	rm -rf libs/

build-linux: a.out
	mkdir game
	mkdir game/engine
	mkdir game/log
	touch game/log/log.log
	mkdir game/engine/data
	mkdir game/engine/data/images
	mkdir game/engine/data/shaders
	cp -r engine/data/*/ game/engine/data/ 
	cp a.out game/game

build-windows: a.exe
	unzip libs.zip
	mkdir game
	mkdir game/engine
	mkdir game/log
	touch game/log/log.log
	mkdir game/engine/data
	mkdir game/engine/data/images
	mkdir game/engine/data/shaders
	cp -r engine/data/*/ game/engine/data/ 
	cp a.exe game/game.exe
	cp -r libs/dlls/* game
