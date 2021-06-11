#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "../../../Settings.h"
#include "../GameObject.h"

typedef struct Scene_s Scene;

typedef enum BlockType_e
{
    BLOCK_GROUND,
    BLOCK_ONE_WAY,
    BLOCK_BRICK,
    BLOCK_BONUS,
    BLOCK_GRAVITY_INVERTER,
    BLOCK_KILL,
    BLOCK_WOOD
} BlockType;

typedef struct Block_s
{
    GameObject *m_object;
    int m_type;
    int m_nbHits;

    PE_Vec2 m_startPos;
    PE_AABB m_startAABB;
} Block;

Block *Block_new(Scene *scene, int type, PE_Vec2 *position, PE_AABB *aabb);

INLINE GameObject *Block_getObject(Block *block)
{
    return block->m_object;
}

void Block_hit(Block *block);

#endif
