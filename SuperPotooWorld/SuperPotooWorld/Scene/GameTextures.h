#ifndef _GAME_TEXTURES_H_
#define _GAME_TEXTURES_H_

#include "../Settings.h"

typedef struct Background_s
{
    
    RE_Texture** layers;
    int layer_count;
    
} Background;

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

    Background* background_default;
    Background* background_01;
    Background* background_02;
    Background* background_03;
    Background* background_04;
    
    RE_Texture *heart;

    // AJOUTS

    RE_Texture* playerRunningFire;
    RE_Texture* playerFallingFire;

    RE_Texture* finish;
    
    RE_Texture* checkpoint_enabled;
    RE_Texture* checkpoint_disabled;

    RE_Texture* PowerUP_Fire;
    RE_Texture* RollingPowerUP_Fire;

    RE_Texture* IdlePlayer;
    RE_Texture* IdleFirePlayer;
    
    RE_Texture* gravityInverter;

    RE_Texture* fireball;
} GameTextures;

GameTextures *GameTextures_new(RE_Renderer *renderer);
void GameTextures_free(GameTextures *textures);
Background* Background_get(GameTextures* textures, int index);


#endif
