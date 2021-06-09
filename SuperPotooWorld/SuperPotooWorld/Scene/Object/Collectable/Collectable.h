#ifndef _COLLECTABLE_H_
#define _COLLECTABLE_H_

#include "../../../Settings.h"
#include "../GameObject.h"

typedef struct Scene_s Scene;

typedef enum CollectableType_e
{
    COLLECTABLE_FIREFLY,
    COLLECTABLE_HEART,

    POWERUP_FIRE
} CollectableType;

typedef struct Collectable_s
{
    GameObject *m_object;
    int m_type;

    PE_Vec2 m_startPos;
} Collectable;

Collectable *Collectable_new(Scene *scene, int type, PE_Vec2 *position);

INLINE GameObject *Collectable_getObject(Collectable *collectable)
{
    return collectable->m_object;
}

#endif
