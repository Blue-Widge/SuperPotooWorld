#ifndef _POWERUP_SCREAM_H_
#define _POWERUP_SCREAM_H_

#include "../../../Settings.h"

typedef struct Collectable_s Collectable;
typedef struct GameObject_s GameObject;

int PowerUP_Scream_onStart(Collectable* collectable);
int PowerUP_Scream_onRespawn(Collectable* collectable);
void PowerUP_Scream_render(Collectable* collectable);
int PowerUP_Scream_fixedUpdate(GameObject *object);

#endif