#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "../../../Settings.h"
#include "../GameObject.h"

typedef struct Scene_s Scene;
typedef enum GravityDirection_e GravityDirection;

typedef enum EnemyType_e
{
    ENEMY_NUT,
} EnemyType;

typedef enum Direction_e
{
    NONE = 0,
    LEFT = -1,
    RIGHT = 1
} DetectionDirection;

typedef struct Enemy_s
{
    GameObject *m_object;
    int m_type;
    int m_state;
    RE_Animator *m_animator;
    PE_Vec2 m_startPos;
    DetectionDirection direction;
    GravityDirection m_gravityDirection;
    
} Enemy;

Enemy *Enemy_new(Scene *scene, int type, PE_Vec2 *position, int gravityDirection);

INLINE GameObject *Enemy_getObject(Enemy *enemy)
{
    return enemy->m_object;
}

void Enemy_damage(Enemy *enemy);

#endif
