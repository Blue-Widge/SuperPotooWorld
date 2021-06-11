#ifndef _SCREAM_H_
#define _SCREAM_H_

#include "../../../Settings.h"

typedef struct Skill_s Skill;

int Scream_onStart(Skill* skill);
int Scream_onRespawn(Skill* skill);
void Scream_render(Skill* skill);
int Scream_fixedUpdate(GameObject* object);

#endif // !_SCREAM_H_