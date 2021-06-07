#ifndef _COLLECTABLE_FIREFLY_H_
#define _COLLECTABLE_FIREFLY_H_

#include "../../../Settings.h"

typedef struct Collectable_s Collectable;

int Firefly_onStart(Collectable *collectable);
int Firefly_onRespawn(Collectable *collectable);
void Firefly_render(Collectable *collectable);

#endif