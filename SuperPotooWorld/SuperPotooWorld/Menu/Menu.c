#include "Menu.h"
#include "MenuInput.h"
#include "MenuTextures.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Menu_createAnimator(Menu *menu)
{
    RE_Animator *animator = NULL;
    MenuTextures *textures = menu->m_textures;
    RE_TextureAnim *texAnim = NULL;
    RE_ParamAnim* param;
    int exitStatus = EXIT_SUCCESS;

    animator = RE_Animator_new();
    if (!animator) goto ERROR_LABEL;

    menu->m_animator = animator;

    texAnim = RE_Animator_createTextureAnim(animator, textures->firefly, "Firefly");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.3f);

    param = RE_Animator_createParamAnim(animator, "xShift");
    if (!param) goto ERROR_LABEL;

    RE_ParamAnim_setShift(param, Vec2_set(-40.f, 0.f), Vec2_set(0.f, 0.f));
    RE_ParamAnim_setFlags(param, RE_ANIM_ALTERNATE);
    RE_ParamAnim_setCycleTime(param, 2.0f);
    RE_ParamAnim_setEasing(param, RE_EasingFct_cos);

    param = RE_Animator_createParamAnim(animator, "yShift");
    if (!param) goto ERROR_LABEL;

    RE_ParamAnim_setShift(param, Vec2_set(0.f, -5.f), Vec2_set(0.f, 5.f));
    RE_ParamAnim_setFlags(param, RE_ANIM_ALTERNATE);
    RE_ParamAnim_setCycleTime(param, 1.4f);
    RE_ParamAnim_setEasing(param, RE_EasingFct_cos);

    // Lancement de l'animation
    exitStatus = RE_Animator_playTextureAnim(animator, "Firefly");
    exitStatus = RE_Animator_playParamAnim(animator, "xShift");
    exitStatus = RE_Animator_playParamAnim(animator, "yShift");

    RE_Animator* Loading = RE_Animator_new();
    texAnim = RE_Animator_createTextureAnim(Loading, textures->loading, "Loading");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 1.f);

    menu->m_loading = Loading;

    param = RE_Animator_createParamAnim(Loading, "Launch");

    exitStatus = RE_Animator_playTextureAnim(Loading, "Loading");

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Menu_createAnimator()\n");
    return EXIT_FAILURE;
}

Menu *Menu_new(RE_Renderer *renderer, RE_Timer *time)
{
    Menu *menu = NULL;
    int nbButtons = 2;
    int width;
    int exitStatus = EXIT_SUCCESS;

    menu = (Menu *)calloc(1, sizeof(Menu));
    if (!menu) goto ERROR_LABEL;

    menu->m_renderer = renderer;
    menu->m_time = time;
    menu->m_nbButtons = nbButtons;
    menu->m_idxOver = -1;
    menu->m_lvlPath = NULL;

    menu->m_textures = MenuTextures_new(renderer);
    if (!menu->m_textures) goto ERROR_LABEL;

    menu->m_input = MenuInput_new();
    if (!menu->m_input) goto ERROR_LABEL;

    menu->m_buttons = (Rect *)calloc(nbButtons, sizeof(Rect));
    if (!menu->m_buttons) goto ERROR_LABEL;

    exitStatus = Menu_createAnimator(menu);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    width = RE_Renderer_getWidth(renderer);
    for (int i = 0; i < nbButtons; i++)
    {
        menu->m_buttons[i] = Rect_set((width - 230.f) / 2.f, 570.f + 75 * i, 230.f, 45.f);
    }
    
    return menu;

ERROR_LABEL:
    printf("ERROR - Menu_new()\n");
    Menu_free(menu);
    return NULL;
}

void Menu_free(Menu *menu)
{
    if (!menu)
        return;

    MenuInput_free(menu->m_input);
    MenuTextures_free(menu->m_textures);
    free(menu->m_buttons);
    free(menu);
}


int Menu_updateLoading(Menu* menu)
{
    MenuInput* input = menu->m_input;
    RE_Timer* time = menu->m_time;

    int accu = 0.f;
    MenuInput_update(input);
    RE_Animator_update(menu->m_loading, time);

    if (input->quitPressed)
        return -1;
    else if (input->buttonPressed)
        return 1;
        
}

void Menu_renderLoading(Menu* menu, Bool Loading)
{
    RE_Renderer* renderer = menu->m_renderer;
    MenuTextures* textures = menu->m_textures;
    int width = RE_Renderer_getWidth(renderer);
    int height = RE_Renderer_getHeight(renderer);
    int w = RE_Texture_getWidth(textures->loading);
    int h = RE_Texture_getHeight(textures->controls);

    RE_Texture_render(textures->background, 0, 0, 0);
    RE_Texture_render(textures->controls, 0, (width / 2) - w/6, height - h);
    RE_Renderer_fill(renderer, RE_Color_set(0, 0, 0, 64));
    if (Loading)
    {
        RE_Animator_render(menu->m_loading, (width / 2) - (w / 28) , height / 2);
    }
    else
    {
        RE_Texture_render(textures->press, 0, (width / 2) - (w / 14), height / 2);
    }
    
}


int Menu_update(Menu *menu)
{
    MenuInput *input = menu->m_input;
    RE_Timer *time = menu->m_time;
    int nbButtons;

    MenuInput_update(input);
    RE_Animator_update(menu->m_animator, time);

    nbButtons = menu->m_nbButtons;

    menu->m_idxOver = -1;
    for (int i = 0; i < nbButtons; i++)
    {
        if (Rect_contains(menu->m_buttons + i, &(input->mousePos)))
        {
            menu->m_idxOver = i;
            break;
        }
    }

    if (input->rightClick)
    {
        switch (menu->m_idxOver)
        {
        case 0:
            menu->m_lvlPath = "../Assets/Levels/Level_01.txt";
            return 1;
            break;

        case 1:
            menu->m_lvlPath = "../Assets/Levels/Level_02.txt";
            return 1;
            break;

        default:
            break;
        }
    }

    if (input->quitPressed)
        return -1;
    else
        return 0;
}

void Menu_render(Menu *menu)
{
    RE_Renderer *renderer = menu->m_renderer;
    MenuTextures *textures = menu->m_textures;
    int width = RE_Renderer_getWidth(renderer);
    int heigth = RE_Renderer_getHeight(renderer);
    int x, y, w, h;

    RE_Texture_render(textures->background, 0, 0, 0);

    RE_Renderer_fill(renderer, RE_Color_set(0, 0, 0, 64));

    w = RE_Texture_getWidth(textures->logo);
    h = RE_Texture_getHeight(textures->logo);
    RE_Texture_render(textures->logo, 0, (width - w) / 2, (heigth - h) / 2 - 180);

    for (int i = 0; i < menu->m_nbButtons; i++)
    {
        x = (int)menu->m_buttons[i].x;
        y = (int)menu->m_buttons[i].y;
        w = RE_Texture_getWidth(textures->level);
        h = RE_Texture_getHeight(textures->level);

        if (menu->m_idxOver == i)
        {
            int fireflyW = RE_Texture_getPartWidth(textures->firefly, 0);
            int fireflyH = RE_Texture_getPartHeight(textures->firefly, 0);
            RE_Animator_render(menu->m_animator, x - 5 - fireflyW, y + (h - fireflyH) / 2);
        }

        RE_Texture_render(textures->level, 0, x, y);
        RE_Texture_render(textures->digits, i + 1, x + w + 15, y);
    }
}
