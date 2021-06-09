#ifndef _POWERUP_FIRE_H_
#define _POWERUP_FIRE_H_

#include "../../../Settings.h"

typedef struct Collectable_s Collectable;
typedef struct GameObject_s GameObject;

int PowerUP_Fire_onStart(Collectable* collectable);
int PowerUP_Fire_onRespawn(Collectable* collectable);
void PowerUP_Fire_render(Collectable* collectable);
int PowerUP_Fire_fixedUpdate(GameObject *object);

#endif