#ifndef _BLOCK_GRAVITY_INVERTER_H_
#define _BLOCK_GRAVITY_INVERTER_H_

#include "../../../Settings.h"

typedef struct Block_s Block;

int GravityInverter_onStart(Block *block);
int GravityInverter_onRespawn(Block *block);
void GravityInverter_render(Block *block);

#endif
