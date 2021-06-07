#ifndef _MENU_INPUT_H_
#define _MENU_INPUT_H_

#include "../Settings.h"

typedef struct MenuInput_s
{
    /// @brief Etat de l'axe horizontal dans l'interval [-1.f, 1.f].
    float hAxis;

    Bool quitPressed; // On ferme fenetre
    Bool rightClick;
    Vec2 mousePos;
} MenuInput;

MenuInput *MenuInput_new();
void MenuInput_free(MenuInput *input);
void MenuInput_update(MenuInput *input);

#endif
