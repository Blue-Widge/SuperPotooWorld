#ifndef _BLOCK_BRICK_H_
#define _BLOCK_BRICK_H_

#include "../../../Settings.h"

typedef struct Block_s Block;

int Brick_onStart(Block *block);
int Brick_onRespawn(Block *block);
void Brick_render(Block *block);
void Brick_hit(Block *block);

#endif