#include "MenuTextures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MenuTextures *MenuTextures_new(RE_Renderer *renderer)
{
    MenuTextures *textures = NULL;
    char imgPath[1024] = "";

    textures = (MenuTextures *)calloc(1, sizeof(MenuTextures));
    if (!textures) goto ERROR_LABEL;

    // Chargement des textures du menu
    textures->logo = RE_Texture_new(
        "../Assets/Images/Menu/Logo.png", renderer);
    if (!textures->logo) goto ERROR_LABEL;

    textures->level = RE_Texture_new(
        "../Assets/Images/Menu/Level.png", renderer);
    if (!textures->level) goto ERROR_LABEL;

    textures->digits = RE_Texture_newSet(
        "../Assets/Images/Menu/Digits.png", renderer, 1, 10);
    if (!textures->digits) goto ERROR_LABEL;

    // Chargement des lucioles
    textures->firefly = RE_Texture_newSet(
        "../Assets/Images/Collectables/Firefly.png", renderer, 2, 4);
    if (!textures->firefly) goto ERROR_LABEL;

    // Chargement du fond
    textures->background = RE_Texture_new(
        "../Assets/Images/Backgrounds/Default/Full.png", renderer);
    if (!textures->background) goto ERROR_LABEL;

    textures->controls = RE_Texture_new(
        "../Assets/Images/Menu/Controls.png", renderer);
    if (!textures->controls) goto ERROR_LABEL;

    textures->loading = RE_Texture_newSet(
        "../Assets/Images/Menu/Loading.png", renderer, 1, 14);
    if (!textures->loading) goto ERROR_LABEL;

    textures->press = RE_Texture_new(
        "../Assets/Images/Menu/press.png", renderer);
    if (!textures->press) goto ERROR_LABEL;

    return textures;

ERROR_LABEL:
    printf("ERROR - MenuTextures_new()\n");
    MenuTextures_free(textures);
    return NULL;
}

void MenuTextures_free(MenuTextures *gameTex)
{
    if (!gameTex)
        return;

    RE_Texture_free(gameTex->logo);
    RE_Texture_free(gameTex->level);
    RE_Texture_free(gameTex->digits);
    RE_Texture_free(gameTex->firefly);
    RE_Texture_free(gameTex->background);
    RE_Texture_free(gameTex->controls);
    RE_Texture_free(gameTex->loading);
    RE_Texture_free(gameTex->press);
    free(gameTex);
}
