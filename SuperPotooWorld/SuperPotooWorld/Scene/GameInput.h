#ifndef _GAME_INPUT_H_
#define _GAME_INPUT_H_

#include "../Settings.h"

typedef struct GameInput_s
{
    /// @brief Etat de l'axe horizontal dans l'interval [-1.f, 1.f].
    float hAxis;

    Bool exitPressed; // On retourne au menu
    Bool quitPressed; // On ferme fenetre
    Bool jumpDown;
    Bool jumpPressed;

    Bool toggleGizmos;
} GameInput;

GameInput *GameInput_new();
void GameInput_free(GameInput *input);
void GameInput_update(GameInput *input);

#endif
