#include "Parallax.h"
#include "Scene.h"

#include <stdio.h>
#include <stdlib.h>

Parallax *Parallax_new(Scene *scene)
{
    Parallax *parallax = NULL;
    GameTextures *textures = NULL;

    textures = Scene_getTextures(scene);

    parallax = (Parallax *)calloc(1, sizeof(Parallax));
    if (!parallax) goto ERROR_LABEL;
    parallax->m_scene = scene;
    parallax->m_width = RE_Texture_getWidth(textures->background_03->layers[0]);
    parallax->m_height = RE_Texture_getHeight(textures->background_03->layers[0]);

    return parallax;

ERROR_LABEL:
    printf("ERROR - Parallax_new()\n");
    Parallax_free(parallax);
    return NULL;
}

void Parallax_free(Parallax *parallax)
{
    if (!parallax)
        return;

    free(parallax);
}

void Parallax_renderBackground(Parallax *parallax)
{
    Scene *scene = parallax->m_scene;
    Camera *camera = Scene_getCamera(scene);
    GameTextures *textures = Scene_getTextures(scene);
    float width = (float)parallax->m_width;
    int camWidth = Camera_getWidth(camera);

    PE_Vec2 position;
    float x, y, xShift;

    PE_Vec2_set(&position, 0.f, 0.f);
    Camera_worldToView(camera, &position, &x, &y);

    Background* background = textures->background_03;
    
    for (int i = 0; i < background->layer_count; ++i)
    {
        xShift = 0.1 / 1 * (i + 1) * x;
        while (xShift < width)
        {
            xShift += width;
        }
        while (xShift > width)
        {
            xShift -= width;
        }
        if (xShift < camWidth)
            RE_Texture_renderF(background->layers[i], 0, xShift, 0);
        if (xShift > 0.f)
            RE_Texture_renderF(background->layers[i], 0, xShift - width, 0);
        
    }
    
    // Ici, le facteur 0.8f permet au fond de se déplacer moins vite que la zone de jeu


}

void Parallax_renderForeground(Parallax *parallax)
{
}
