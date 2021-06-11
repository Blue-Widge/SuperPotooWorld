#ifndef _BLOCK_KILL_H_
#define _BLOCK_KILL_H_

#include "../../../Settings.h"

typedef struct Block_s Block;

int Kill_onStart(Block *block);
int Kill_onRespawn(Block *block);
void Kill_render(Block *block);

#endif