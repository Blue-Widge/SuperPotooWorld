#include "Block.h"
#include "Block_Ground.h"
#include "Block_Brick.h"
#include "Block_OneWay.h"
#include "Block_Bonus.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

// Méthodes du GameObject
int Block_free(GameObject *object);
int Block_onStart(GameObject *object);
int Block_onRespawn(GameObject *object);
int Block_render(GameObject *object);

Block *Block_new(Scene *scene, int type, PE_Vec2 *position, PE_AABB *aabb)
{
    GameObject *object = NULL;
    Block *block = NULL;

    object = GameObject_new(scene, GAME_BLOCK);
    if (!object) goto ERROR_LABEL;

    block = (Block *)calloc(1, sizeof(Block));
    if (!block) goto ERROR_LABEL;

    block->m_object = object;
    block->m_type = type;
    block->m_startPos = *position;
    block->m_startAABB = *aabb;

    // Initialisation de la classe mère
    object->m_data = (void *)block;
    object->m_layer = 0;
    object->cm_free = Block_free;
    object->cm_onStart = Block_onStart;
    object->cm_onRespawn = Block_onRespawn;
    object->cm_render = Block_render;

    return block;

ERROR_LABEL:
    printf("ERROR - Block_new()\n");
    GameObject_free(object);
    return NULL;
}

int Block_free(GameObject *object)
{
    Block *block = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    block = GameObject_getBlock(object);
    if (!block) goto ERROR_LABEL;
    
    free(block);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Block_free()\n");
    return EXIT_FAILURE;
}

int Block_onStart(GameObject *object)
{
    Block *block = NULL;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    block = GameObject_getBlock(object);
    if (!block) goto ERROR_LABEL;

    switch (block->m_type)
    {
    case BLOCK_GROUND:
        exitStatus = Ground_onStart(block);
        break;

    case BLOCK_ONE_WAY:
        exitStatus = OneWay_onStart(block);
        break;

    case BLOCK_BRICK:
        exitStatus = Brick_onStart(block);
        break;

    case BLOCK_BONUS:
        exitStatus = Bonus_onStart(block);
        break;

    default:
        printf("ERROR - Unknown block type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Block_onStart()\n");
    return EXIT_FAILURE;
}

int Block_onRespawn(GameObject *object)
{
    Block *block = NULL;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    block = GameObject_getBlock(object);
    if (!block) goto ERROR_LABEL;

    switch (block->m_type)
    {
    case BLOCK_BRICK:
        exitStatus = Brick_onRespawn(block);
        break;

    case BLOCK_BONUS:
        exitStatus = Bonus_onRespawn(block);
        break;

    case BLOCK_GROUND:
    case BLOCK_ONE_WAY:
        printf("ERROR - This block cannot be respawn\n");
        goto ERROR_LABEL;
        break;

    default:
        printf("ERROR - Unknown block type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Block_onRespawn()\n");
    return EXIT_FAILURE;
}

int Block_render(GameObject *object)
{
    Block *block = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    block = GameObject_getBlock(object);
    if (!block) goto ERROR_LABEL;

    switch (block->m_type)
    {
    case BLOCK_BRICK:
        Brick_render(block);
        break;

    case BLOCK_BONUS:
        Bonus_render(block);
        break;

    default:
        break;
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Block_render()\n");
    return EXIT_FAILURE;
}

void Block_hit(Block *block)
{
    switch (block->m_type)
    {
    case BLOCK_BRICK:
        Brick_hit(block);
        break;

    case BLOCK_BONUS:
        Bonus_hit(block);
        break;

    default:
        break;
    }
}
