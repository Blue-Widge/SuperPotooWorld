#ifndef _GAME_ANIMATORS_H_
#define _GAME_ANIMATORS_H_

#include "../Settings.h"

typedef struct Scene_s Scene;

typedef struct GameAnimators_s
{
    Scene *m_scene;

    RE_Animator *firefly;
    RE_Animator *bonus;
    RE_Animator* RollingPowerUP_Fire;
    
} GameAnimators;

GameAnimators *GameAnimators_new(Scene *scene);
void GameAnimators_free(GameAnimators *animators);

void GameAnimators_update(GameAnimators *animators);

#endif
