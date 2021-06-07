#ifndef _COLLECTABLE_HEART_H_
#define _COLLECTABLE_HEART_H_

#include "../../../Settings.h"

typedef struct Collectable_s Collectable;

int Heart_onStart(Collectable *collectable);
int Heart_onRespawn(Collectable *collectable);
void Heart_render(Collectable *collectable);

#endif