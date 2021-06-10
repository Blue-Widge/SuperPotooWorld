#include "GameUI.h"
#include "Scene.h"
#include "Object/Player.h"
#include "../UITextures.h"

#include <stdio.h>
#include <stdlib.h>

void GameUI_render(Scene *scene, PlayerStats *stats)
{
    // UI : User Interface

    // TODO
    // Affichez l'interface utilisateur du jeu.
    // Par exemple les stats du joueur (vie, coeur, lucioles).
    // Eventuellement le temps restant, la progression dans le niveau...
      
    RE_Renderer* renderer = scene->m_renderer;
    UITextures* textures = scene->m_UItextures;
    int width = RE_Renderer_getWidth(renderer);
    int heigth = RE_Renderer_getHeight(renderer);
    int x, y, w, h;
    int nbLives = stats->nbLives;
    int nbHearts = stats->nbHearts;
    int nbFireflies = stats->nbFireflies;
    int widthLives = RE_Texture_getWidth(textures->Lives);
    w = widthLives;
    h = RE_Texture_getHeight(textures->Lives);
    int digitWidth = RE_Texture_getWidth(textures->digits) / 10;

    switch (stats->PowerUP)
    {
    case PLAYER_FIRE:
        RE_Texture_render(textures->LivesFire, 0, 5, 2);
        break;
    default:
        RE_Texture_render(textures->Lives, 0, 5, 2);
        break;
    }
    RE_Texture_render(textures->Fireflies, 1, 5, 12 + h);

    int nbDigitsLives = 0, nbDigitsFireflies = 0, temp = nbLives;
    while (temp)
    {
        temp /= 10;
        nbDigitsLives++;
    }

    temp = nbFireflies;

    while (temp)
    {
        temp /= 10;
        nbDigitsFireflies++;
    }

    int widthfinalHeart = (nbDigitsLives - 1) * digitWidth + widthLives;
    int widthfinal = (nbDigitsFireflies - 1) * digitWidth + widthLives;
    w = widthfinal;
    for (; nbDigitsLives > 0; --nbDigitsLives)
    {
        RE_Texture_render(textures->digits, nbLives % 10 , w + 10, 2 + h / 2);
        w -= digitWidth;
        nbLives /= 10;
    }

    w = widthfinalHeart + 30;
    for (; nbHearts >= 0; nbHearts--)
    {
        RE_Texture_render(textures->Hearts, 0, w + 5, h / 4);
        w += RE_Texture_getWidth(textures->Hearts);
    }
    w = widthfinal;
    for (; nbDigitsFireflies > 0; --nbDigitsFireflies)
    {
        RE_Texture_render(textures->digits, nbFireflies % 10, w + 10, 24 + h);
        w -= digitWidth;
        nbFireflies /= 10;
    }
}
