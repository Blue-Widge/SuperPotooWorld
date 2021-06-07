#include "GameTextures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GameTextures *GameTextures_new(RE_Renderer *renderer)
{
    GameTextures *textures = NULL;
    char imgPath[1024] = "";

    textures = (GameTextures *)calloc(1, sizeof(GameTextures));
    if (!textures) goto ERROR_LABEL;

    // TODO
    // Chargez de nouvelles textures en fonction de vos besoins

    // Chargement des textures de la tilemap
    textures->ground = RE_Texture_newSet(
        "../Assets/Images/Map/Ground.png", renderer, 6, 8);
    if (!textures->ground) goto ERROR_LABEL;

    textures->wood = RE_Texture_new(
        "../Assets/Images/Map/Wood.png", renderer);
    if (!textures->wood) goto ERROR_LABEL;

    textures->oneWay = RE_Texture_new(
        "../Assets/Images/Map/OneWay.png", renderer);
    if (!textures->oneWay) goto ERROR_LABEL;

    // Chargement des textures du joueur
    textures->playerRunning = RE_Texture_newSet(
        "../Assets/Images/Player/Running.png", renderer, 1, 6);
    if (!textures->playerRunning) goto ERROR_LABEL;

    textures->playerFalling = RE_Texture_newSet(
        "../Assets/Images/Player/Falling.png", renderer, 1, 8);
    if (!textures->playerFalling) goto ERROR_LABEL;

    // Chargement des lucioles
    textures->firefly = RE_Texture_newSet(
        "../Assets/Images/Collectables/Firefly.png", renderer, 2, 4);
    if (!textures->firefly) goto ERROR_LABEL;

    // Chargement du fond
    textures->background = RE_Texture_new(
        "../Assets/Images/Background/Full.png", renderer);
    if (!textures->background) goto ERROR_LABEL;

    return textures;

ERROR_LABEL:
    printf("ERROR - GameTextures_new()\n");
    GameTextures_free(textures);
    return NULL;
}

void GameTextures_free(GameTextures *textures)
{
    if (!textures)
        return;

    // TODO
    // Supprimez vos nouvelles textures

    RE_Texture_free(textures->ground);
    RE_Texture_free(textures->wood);
    RE_Texture_free(textures->oneWay);
    RE_Texture_free(textures->playerRunning);
    RE_Texture_free(textures->playerFalling);
    RE_Texture_free(textures->firefly);
    RE_Texture_free(textures->background);

    free(textures);
}
