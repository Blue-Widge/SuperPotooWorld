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
    RE_Texture* background;
    RE_Texture* controls;

    RE_Texture* loading;
    RE_Texture* press;
} MenuTextures;

MenuTextures *MenuTextures_new(RE_Renderer *renderer);
void MenuTextures_free(MenuTextures *textures);

#endif