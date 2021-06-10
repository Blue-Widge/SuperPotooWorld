#ifndef _MENU_H_
#define _MENU_H_

#include "../Settings.h"

#include "MenuTextures.h"
#include "MenuInput.h"

typedef struct Menu_s
{
    RE_Renderer *m_renderer;
    RE_Timer *m_time;
    MenuTextures *m_textures;
    MenuInput *m_input;
    char *m_lvlPath;

    Rect *m_buttons;
    int m_nbButtons;
    int m_idxOver;

    RE_Animator *m_animator;
    RE_Animator* m_loading;
} Menu;


Menu *Menu_new(RE_Renderer *renderer, RE_Timer *time);
void Menu_free(Menu *menu);

int Menu_update(Menu* menu);
void Menu_render(Menu* menu);

int Menu_updateLoading(Menu* menu);
void Menu_renderLoading(Menu* menu, Bool Loading);

INLINE char *Menu_getLevelPath(Menu *menu)
{
    return menu->m_lvlPath;
}

#endif
