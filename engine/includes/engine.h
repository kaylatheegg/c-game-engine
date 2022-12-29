#pragma once

#define ENGINE_H

char* error;

#define NOVALUE 0xFFFFFFFF
//CONFIG OPTIONS!!!

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define MAX_RENDER_LAYERS 32


#define STACK_TRACE_LIMIT 25

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifndef __WIN32__ 
#include <execinfo.h>
#include <signal.h>
#include <errno.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H 

#include "libs/chipmunk/chipmunk.h"

#ifdef __WIN32__
typedef unsigned int uint;
#endif

#include "vector/vector_header.h"
#include "utils/utils_header.h"

#include "init/init_header.h"
#include "log/log_header.h"
#include "render/renderer_header.h"
#include "event/event_header.h"
#include "object/object_header.h"
#include "entity/entity_header.h"
#include "entities/entities_header.h"
#include "ui/ui_header.h"
#include "audio/audio_header.h"

#include "world/world_header.h"



//notes:

//dt is fractions of a second 
