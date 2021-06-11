

#include <stdio.h>
#include <stdlib.h>

#include "../Scene/Scene.h"

void GameUI_render(Scene *scene, PlayerStats *stats)
{
    // UI : User Interface
      
    RE_Renderer* renderer = scene->m_renderer;
    UITextures* textures = scene->m_UItextures;
    int width = RE_Renderer_getWidth(renderer);
    int heigth = RE_Renderer_getHeight(renderer);
    int x, y, w, h;
    int nbLives = stats->nbLives;
    int nbHearts = stats->nbHearts;
    int nbFireflies = stats->nbFireflies;
    w = RE_Texture_getWidth(textures->Lives);
    h = RE_Texture_getHeight(textures->Lives);
    int digitWidth = RE_Texture_getWidth(textures->digits) / 10;
    int heartWidth = RE_Texture_getWidth(textures->Hearts);
    switch (stats->PowerUP)
    {
    case PLAYER_FIRE:
        RE_Texture_render(textures->LivesFire, 0, 5, 2);
        break;
        case PLAYER_SCREAM:
            RE_Texture_render(textures->LivesScream, 0, 5, 2);
        break;
    default:
        RE_Texture_render(textures->Lives, 0, 5, 2);
        break;
    }
    RE_Texture_render(textures->Fireflies, 1, 5, h + 20);
    
    w += heartWidth;
    for (; nbHearts >= 0; --nbHearts)
    {
        RE_Texture_render(textures->Hearts, 0, w + 5, h / 4);
        w += heartWidth;
    }

    w = RE_Texture_getWidth(textures->Lives);
    if (nbFireflies > 9)
    {
        RE_Texture_render(textures->digits, nbFireflies / 10, w + 5, 40 + h);
        w += digitWidth;
        RE_Texture_render(textures->digits, nbFireflies % 10, w + 5, 40 + h);
         w += digitWidth;
    }
    else
    {
        RE_Texture_render(textures->digits, 0, w + 5, 40 + h);
        w += digitWidth;
        RE_Texture_render(textures->digits, nbFireflies, w + 5, 40 + h);
        w += digitWidth;
    }
}
