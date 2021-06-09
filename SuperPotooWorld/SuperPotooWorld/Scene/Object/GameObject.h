#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "../../Settings.h"

typedef struct Scene_s Scene;
typedef struct Player_s Player;
typedef struct Block_s Block;
typedef struct Collectable_s Collectable;
typedef struct Camera_s Camera;
typedef struct Enemy_s Enemy;
typedef struct Tool_s Tool;

typedef enum GameObjectType_e
{
    GAME_PLAYER,
    GAME_BLOCK,
    GAME_COLLECTABLE,
    GAME_ENEMY,
    GAME_CAMERA,
    GAME_TOOL
} GameObjectType;

typedef enum CollisionFilter_e
{
    FILTER_PLAYER      = 1 << 0,
    FILTER_BLOCK       = 1 << 1,
    FILTER_COLLECTABLE = 1 << 2,
    FILTER_ENEMY       = 1 << 3,
    FILTER_CAMERA      = 1 << 4,
    FILTER_TOOL        = 1 << 5,
    FILTER_VISIBLE     = 1 << 6,
    FILTER_DAMAGEABLE  = 1 << 7,
    FILTER_DAMAGER     = 1 << 8
} CollisionFilter;

typedef enum GameObjectFlag_e
{
    OBJECT_TO_START   = 1 << 0,
    OBJECT_TO_REMOVE  = 1 << 1,
    OBJECT_TO_DISABLE = 1 << 2,
    OBJECT_ALL_FLAGS  = ~0 
} GameObjectFlag;

typedef struct GameObject_s GameObject;

typedef int (*GOMethod)(GameObject *);

typedef struct GameObject_s
{
    Scene *m_scene;
    int m_type;
    int m_flags;
    int m_layer;
    PE_Body *m_body;
    void *m_data;

    GOMethod cm_free;
    GOMethod cm_onRespawn;
    GOMethod cm_onStart;
    GOMethod cm_update;
    GOMethod cm_fixedUpdate;
    GOMethod cm_render;
} GameObject;

GameObject *GameObject_new(Scene *scene, int type);
void GameObject_free(GameObject *object);

// Propriétés de bases
INLINE void GameObject_setBody(GameObject *object, PE_Body *body)
{
    if (body)
    {
        object->m_body = body;
        PE_Body_setUserData(body, object);
    }
}

INLINE PE_Body *GameObject_getBody(GameObject *object)
{
    return object->m_body;
}

void GameObject_removeBody(GameObject *object);

INLINE int GameObject_getType(GameObject *object)
{
    return object->m_type;
}

INLINE Scene *GameObject_getScene(GameObject *object)
{
    return object->m_scene;
}

PE_Vec2 GameObject_getPosition(GameObject *object);

PE_Vec2 GameObject_getVelocity(GameObject *object);


// Accès au classes filles
INLINE Player *GameObject_getPlayer(GameObject *object)
{
    if (object->m_type == GAME_PLAYER)
        return (Player *)object->m_data;
    else
        return NULL;
}

INLINE Collectable *GameObject_getCollectable(GameObject *object)
{
    if (object->m_type == GAME_COLLECTABLE)
        return (Collectable *)object->m_data;
    else
        return NULL;
}

INLINE Enemy *GameObject_getEnemy(GameObject *object)
{
    if (object->m_type == GAME_ENEMY)
        return (Enemy *)object->m_data;
    else
        return NULL;
}

INLINE Block *GameObject_getBlock(GameObject *object)
{
    if (object->m_type == GAME_BLOCK)
        return (Block *)object->m_data;
    else
    {
        printf("ERROR - This object is not a block\n");
        return NULL;
    }
}

INLINE Camera *GameObject_getCamera(GameObject *object)
{
    if (object->m_type == GAME_CAMERA)
        return (Camera *)object->m_data;
    else
        return NULL;
}

INLINE Tool *GameObject_getTool(GameObject *object)
{
    if (object->m_type == GAME_TOOL)
        return (Tool *)object->m_data;
    else
        return NULL;
}

// Fonction génériques
int GameObject_onRespawn(GameObject *object);
int GameObject_onStart(GameObject *object);
int GameObject_update(GameObject *object);
int GameObject_fixedUpdate(GameObject *object);
int GameObject_render(GameObject *object);

// Gestion des flags
INLINE Bool GameObject_hasOneFlag(GameObject *object, int flags)
{
    return (object->m_flags & flags);
}

INLINE void GameObject_addFlags(GameObject *object, int flags)
{
    object->m_flags |= flags;
}

INLINE void GameObject_removeFlags(GameObject *object, int flags)
{
    object->m_flags &= ~flags;
}

#endif