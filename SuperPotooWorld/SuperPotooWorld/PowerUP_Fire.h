#ifndef _POWERUP_FIRE_H_
#define _POWERUP_FIRE_H_

#include "../SuperPotooWorld/Settings.h"

typedef struct Collectable_s Collectable;

int PowerUP_Fire_onStart(Collectable* collectable);
int PowerUP_Fire_onRespawn(Collectable* collectable);
void PowerUP_Fire_render(Collectable* collectable);

#endif