#pragma once

#define ENGINE_H

char* error;

#define NOVALUE 0xFFFFFFFF

//CONFIG OPTIONS!!!

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define STACK_TRACE_LIMIT 25


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <execinfo.h>
#include <signal.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>



#include "utils/utils_header.h"
#include "vector/vector_header.h"

#include "init/init_header.h"
#include "log/log_header.h"
#include "render/renderer_header.h"
#include "event/event_header.h"
#include "object/object_header.h"
#include "entity/entity_header.h"
#include "entities/entities_header.h"
