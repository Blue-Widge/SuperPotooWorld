#ifndef _SKILL_H_
#define _SKILL_H_

#include "../../../Settings.h"
#include "../GameObject.h"

typedef struct Scene_s Scene;

typedef enum SkillType_e
{
    FIREBALL

} SkillType;

typedef struct Skill_s
{
    GameObject* m_object;
    int m_type;
    PE_Vec2 m_startPos;
    int direction;
    RE_Animator* animator;
    
} Skill;

Skill* Skill_new(Scene* scene, int type, PE_Vec2* position);

INLINE GameObject* Skill_getObject(Skill* skill)
{
    return skill->m_object;
}

#endif 