#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "../../../Settings.h"
#include "../GameObject.h"

typedef struct Scene_s Scene;

typedef enum EnemyType_e
{
    ENEMY_NUT,
} EnemyType;

typedef enum Direction_e
{
    NONE,
    LEFT,
    RIGHT
} DetectionDirection;

typedef struct Enemy_s
{
    GameObject *m_object;
    int m_type;
    int m_state;
    RE_Animator *m_animator;
    PE_Vec2 m_startPos;
    DetectionDirection direction;
    
} Enemy;

Enemy *Enemy_new(Scene *scene, int type, PE_Vec2 *position);

INLINE GameObject *Enemy_getObject(Enemy *enemy)
{
    return enemy->m_object;
}

void Enemy_damage(Enemy *enemy);

#endif
