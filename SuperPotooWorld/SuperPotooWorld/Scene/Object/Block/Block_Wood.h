#ifndef _BLOCK_WOOD_H_
#define _BLOCK_WOOD_H_

#include "../../../Settings.h"

typedef struct Block_s Block;

int Wood_onStart(Block *block);
int Wood_onRespawn(Block *block);
void Wood_render(Block *block);

#endif