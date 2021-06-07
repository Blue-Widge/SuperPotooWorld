#ifndef _BLOCK_BONUS_H_
#define _BLOCK_BONUS_H_

#include "../../../Settings.h"

typedef struct Block_s Block;

int Bonus_onStart(Block *block);
int Bonus_onRespawn(Block *block);
void Bonus_render(Block *block);
void Bonus_hit(Block *block);

#endif