#ifndef _RENDERING_ENGINE_H_
#define _RENDERING_ENGINE_H_

#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include "RE_Lib.h"

#include "RE_Animator.h"
#include "RE_HashTable.h"
#include "RE_Renderer.h"
#include "RE_Texture.h"
#include "RE_Timer.h"
#include "RE_Utils.h"
#include "RE_Window.h"

#endif