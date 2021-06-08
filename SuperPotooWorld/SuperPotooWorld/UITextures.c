#include "UITextures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

UITextures* UITextures_new(RE_Renderer* renderer)
{
    UITextures* textures = NULL;
    char imgPath[1024] = "";

    textures = (UITextures*)calloc(1, sizeof(UITextures));
    if (!textures) goto ERROR_LABEL;

    // Chargement des textures du menu
    textures->Lives = RE_Texture_new(
        "../Assets/Images/Player/Idle.png", renderer);  
    if (!textures->Lives) goto ERROR_LABEL;

    textures->Hearts = RE_Texture_new(
        "../Assets/Images/Collectables/Heart.png", renderer);
    if (!textures->Hearts) goto ERROR_LABEL;

    textures->digits = RE_Texture_newSet(
        "../Assets/Images/Menu/Digits.png", renderer, 1, 10);
    if (!textures->digits) goto ERROR_LABEL;

    return textures;

ERROR_LABEL:
    printf("ERROR - UITextures_new()\n");
    MenuTextures_free(textures);
    return NULL;
}

void UITexture_free(UITextures* UITex)
{
    if (!UITex)
        return;

    RE_Texture_free(UITex->Lives);
    RE_Texture_free(UITex->Hearts);
    RE_Texture_free(UITex->digits);

    free(UITex);
}
