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
    
    textures->brick = RE_Texture_new(
        "../Assets/Images/Map/Brick.png", renderer);
    if (!textures->brick) goto ERROR_LABEL;
    
    textures->background_layer_0 = RE_Texture_new(
        "../Assets/Images/Background/Layer_00.png", renderer);
    if (!textures->background_layer_0) goto ERROR_LABEL;
    
    textures->background_layer_1 = RE_Texture_new(
        "../Assets/Images/Background/Layer_01.png", renderer);
    if (!textures->background_layer_1) goto ERROR_LABEL;
    
    textures->background_layer_2 = RE_Texture_new(
        "../Assets/Images/Background/Layer_02.png", renderer);
    if (!textures->background_layer_2) goto ERROR_LABEL;
    
    textures->background_layer_3 = RE_Texture_new(
        "../Assets/Images/Background/Layer_03.png", renderer);
    if (!textures->background_layer_3) goto ERROR_LABEL;
    
    textures->background_layer_4 = RE_Texture_new(
        "../Assets/Images/Background/Layer_04.png", renderer);
    if (!textures->background_layer_4) goto ERROR_LABEL;
    
    textures->bonus = RE_Texture_newSet(
        "../Assets/Images/Map/Bonus.png", renderer, 4, 4);
    if (!textures->bonus) goto ERROR_LABEL;
    
    textures->hazelnut_dead = RE_Texture_new(
        "../Assets/Images/Enemies/HazelnutDead.png", renderer);
    if (!textures->hazelnut_dead) goto ERROR_LABEL;

    textures->hazelnut = RE_Texture_newSet(
        "../Assets/Images/Enemies/Hazelnut.png", renderer, 1, 4);
    if (!textures->hazelnut) goto ERROR_LABEL;
    
    textures->heart = RE_Texture_new(
        "../Assets/Images/Collectables/Heart.png", renderer);
    if (!textures->heart) goto ERROR_LABEL;
    
    textures->bonus_empty = RE_Texture_new(
        "../Assets/Images/Map/BonusEmpty.png", renderer);
    if (!textures->bonus_empty) goto ERROR_LABEL;

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


    // AJOUTS

    textures->playerRunningFire = RE_Texture_newSet(
        "../Assets/Images/Player/RunningFire.png", renderer, 1, 6);
    if (!textures->playerRunningFire) goto ERROR_LABEL;

    textures->playerFallingFire = RE_Texture_newSet(
        "../Assets/Images/Player/FallingFire.png", renderer, 1, 8);
    if (!textures->playerFallingFire) goto ERROR_LABEL;


    textures->PowerUP_Fire = RE_Texture_new(
        "../Assets/Images/Collectables/PowerUP_Fire.png", renderer);
    if (!textures->PowerUP_Fire) goto ERROR_LABEL;

    textures->RollingPowerUP_Fire = RE_Texture_newSet(
        "../Assets/Images/Collectables/RollingPowerUP_Fire.png", renderer, 1, 8);
    if (!textures->RollingPowerUP_Fire) goto ERROR_LABEL;

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

    RE_Texture_free(textures->ground);
    RE_Texture_free(textures->wood);
    RE_Texture_free(textures->oneWay);
    RE_Texture_free(textures->playerRunning);
    RE_Texture_free(textures->playerFalling);
    RE_Texture_free(textures->firefly);
    RE_Texture_free(textures->background);
    RE_Texture_free(textures->brick);
    RE_Texture_free(textures->bonus_empty);
    RE_Texture_free(textures->bonus);
    RE_Texture_free(textures->hazelnut);
    RE_Texture_free(textures->hazelnut_dead);
    RE_Texture_free(textures->heart);
    RE_Texture_free(textures->background_layer_0);
    RE_Texture_free(textures->background_layer_1);
    RE_Texture_free(textures->background_layer_2);
    RE_Texture_free(textures->background_layer_3);
    RE_Texture_free(textures->background_layer_4);

    //AJOUTS
    RE_Texture_free(textures->playerRunningFire);
    RE_Texture_free(textures->playerFallingFire);

    RE_Texture_free(textures->PowerUP_Fire);
    RE_Texture_free(textures->RollingPowerUP_Fire);

    free(textures);
}
