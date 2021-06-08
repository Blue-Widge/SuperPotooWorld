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

    w = RE_Texture_getWidth(textures->Lives);
    h = RE_Texture_getHeight(textures->Lives);

    RE_Texture_render(textures->Lives, 0, 5, 2);
    
    RE_Texture_render(textures->digits, nbLives, w + 10, 2 + h / 2);

    w += RE_Texture_getWidth(textures->digits) / 10 + 30;
    for (; nbHearts >= 0; nbHearts--)
    {
        RE_Texture_render(textures->Hearts, 0, w + 5, h / 4);
        w += RE_Texture_getWidth(textures->Hearts);
    }
}
