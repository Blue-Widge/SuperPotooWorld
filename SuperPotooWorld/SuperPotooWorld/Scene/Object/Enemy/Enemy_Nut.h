#ifndef _ENEMY_NUT_H_
#define _ENEMY_NUT_H_

#include "../../../Settings.h"

typedef struct Enemy_s Enemy;

typedef enum NutState_e
{
    NUT_ASLEEP,
    NUT_STARTING,
    NUT_SPINNING,
} NutState;

int Nut_init(Enemy *enemy);
int Nut_onStart(Enemy *enemy);
int Nut_onRespawn(Enemy *enemy);
int Nut_fixedUpdate(Enemy *enemy);
void Nut_render(Enemy *enemy);

void Nut_damage(Enemy *enemy);

#endif