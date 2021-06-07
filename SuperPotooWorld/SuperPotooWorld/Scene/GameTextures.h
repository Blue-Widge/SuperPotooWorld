#ifndef _GAME_TEXTURES_H_
#define _GAME_TEXTURES_H_

#include "../Settings.h"

typedef struct GameTextures_s
{
    RE_Texture *ground;
    RE_Texture *wood;
    RE_Texture *oneWay;

    RE_Texture *playerRunning;
    RE_Texture *playerFalling;

    RE_Texture *firefly;

    RE_Texture *background;
} GameTextures;

GameTextures *GameTextures_new(RE_Renderer *renderer);
void GameTextures_free(GameTextures *textures);

#endif
