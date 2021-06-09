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
    RE_Texture *brick;

    RE_Texture* hazelnut;
    RE_Texture* hazelnut_dead;

    RE_Texture *bonus;
    RE_Texture *bonus_empty;
  
    RE_Texture *background;

    RE_Texture* background_layer_0;
    RE_Texture* background_layer_1;
    RE_Texture* background_layer_2;
    RE_Texture* background_layer_3;
    RE_Texture* background_layer_4;
    
    RE_Texture *heart;

    // AJOUTS

    RE_Texture* playerRunningFire;
    RE_Texture* playerFallingFire;

    RE_Texture* PowerUP_Fire;
    RE_Texture* RollingPowerUP_Fire;
} GameTextures;

GameTextures *GameTextures_new(RE_Renderer *renderer);
void GameTextures_free(GameTextures *textures);

#endif
