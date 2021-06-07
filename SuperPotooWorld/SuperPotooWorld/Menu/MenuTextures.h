#ifndef _MENU_TEXTURES_H_
#define _MENU_TEXTURES_H_

#include "../Settings.h"

#define NB_LAYERS 5

typedef struct MenuTextures_s
{
    RE_Texture *logo;
    RE_Texture *level;
    RE_Texture *digits;

    RE_Texture *firefly;
    RE_Texture *background;
} MenuTextures;

MenuTextures *MenuTextures_new(RE_Renderer *renderer);
void MenuTextures_free(MenuTextures *textures);

#endif