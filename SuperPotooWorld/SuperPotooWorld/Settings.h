#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#ifdef _WIN32

#include <PE.h>
#include <RE.h>
#include <SDL.h>
#include <SDL_image.h>

#define INLINE inline

#else

#include "../PlatformerEngine/PE.h"
#include "../RenderingEngine/RE.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define INLINE static inline

#endif

#endif
