#ifndef _FIREBALL_H_
#define _FIREBALL_H_

#include "../../../Settings.h"

typedef struct Skill_s Skill;

int FireBall_onStart(Skill* skill);
int FireBall_onRespawn(Skill* skill);
void FireBall_render(Skill* skill);
int Fireball_fixedUpdate(GameObject* object);

#endif // !_FIREBALL_H_