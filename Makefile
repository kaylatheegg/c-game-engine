#finds all files in engine/source in all directories that have the .c suffix
SRCPATHS = engine/src/*/*.c

CSRC = $(wildcard $(SRCPATHS))

OBJECTS = $(CSRC:.c=.o)

#LCC is our linux compiler
LCC = gcc

#WCC is our windows compiler
WCC = x86_64-w64-mingw32-gcc-win32

#LCFLAGS are our compile time flags for linux
LCFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLEW -Iengine/includes 

DEBUGFLAGS = -g -rdynamic

DONTBEAFUCKINGIDIOT = -Werror -Wall -Wextra -pedantic -Wno-missing-field-initializers

#WCFLAGS are our compile time flags for windows
WCFLAGS = -lmingw32 -lm -mwindows -Ilibs/ -Llibs/SDL2/ -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libgcc -static-libstdc++ -Werror

#i dont know what these do, they appear to "support" the compilation
WSFLAGS = -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid 

%.o: %.c
	$(LCC) -c -o $@ $< $(LCFLAGS) $(DEBUGFLAGS) $(DONTBEAFUCKINGIDIOT)

a.out : $(OBJECTS)
	$(LCC) $(OBJECTS) $(LCFLAGS) $(DEBUGFLAGS) $(DONTBEAFUCKINGIDIOT)

a.exe : $(CSRC)
	$(WCC) $(CSRC) $(WCFLAGS) $(WSFLAGS)

clean: 
	rm -f engine/src/*/*.o
	rm -f *.o

