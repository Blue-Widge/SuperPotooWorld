#include "GameTextures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Background* load_background_01(RE_Renderer *renderer)
{

    Background* background = calloc(1, sizeof(Background*));
    background->layer_count = 7;
    background->layers = calloc(background->layer_count, sizeof(RE_Texture**));

    background->layers[0] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 1/layers/sky.png", renderer);
    if (!background->layers[0]) goto ERROR_LABEL;

    background->layers[1] = RE_Texture_new(
    "../Assets/Images/Backgrounds/Background 1/layers/clouds_4.png", renderer);
    if (!background->layers[1]) goto ERROR_LABEL;

    background->layers[2] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 1/layers/clouds_3.png", renderer);
    if (!background->layers[2]) goto ERROR_LABEL;

    background->layers[3] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 1/layers/clouds_2.png", renderer);
    if (!background->layers[3]) goto ERROR_LABEL;

    background->layers[4] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 1/layers/clouds_1.png", renderer);
    if (!background->layers[4]) goto ERROR_LABEL;

    background->layers[5] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 1/layers/rocks_1.png", renderer);
    if (!background->layers[5]) goto ERROR_LABEL;

    background->layers[6] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 1/layers/rocks_2.png", renderer);
    if (!background->layers[6]) goto ERROR_LABEL;

    return background;

ERROR_LABEL:
    return NULL;
    
}

Background* load_background_02(RE_Renderer *renderer)
{

    Background* background = calloc(1, sizeof(Background*));
    background->layer_count = 6;
    background->layers = calloc(background->layer_count, sizeof(RE_Texture**));

    background->layers[0] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 2/layers/sky.png", renderer);
    if (!background->layers[0]) goto ERROR_LABEL;

    background->layers[1] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 2/layers/clouds_1.png", renderer);
    if (!background->layers[1]) goto ERROR_LABEL;

    background->layers[2] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 2/layers/rocks_3.png", renderer);
    if (!background->layers[2]) goto ERROR_LABEL;

    background->layers[3] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 2/layers/rocks_2.png", renderer);
    if (!background->layers[3]) goto ERROR_LABEL;

    background->layers[4] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 2/layers/rocks_1.png", renderer);
    if (!background->layers[4]) goto ERROR_LABEL;

    background->layers[5] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 2/layers/pines.png", renderer);
    if (!background->layers[5]) goto ERROR_LABEL;

    return background;

ERROR_LABEL:
    return NULL;
    
}

Background* load_background_03(RE_Renderer *renderer)
{

    Background* background = calloc(1, sizeof(Background*));
    background->layer_count = 8;
    background->layers = calloc(background->layer_count, sizeof(RE_Texture**));

    background->layers[0] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 3/layers/sky.png", renderer);
    if (!background->layers[0]) goto ERROR_LABEL;

    background->layers[1] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 3/layers/rocks.png", renderer);
    if (!background->layers[1]) goto ERROR_LABEL;

    background->layers[2] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 3/layers/clouds_1.png", renderer);
    if (!background->layers[2]) goto ERROR_LABEL;

    background->layers[3] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 3/layers/clouds_2.png", renderer);
    if (!background->layers[3]) goto ERROR_LABEL;

    background->layers[4] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 3/layers/ground_1.png", renderer);
    if (!background->layers[4]) goto ERROR_LABEL;

    background->layers[5] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 3/layers/ground_2.png", renderer);
    if (!background->layers[5]) goto ERROR_LABEL;

    background->layers[6] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 3/layers/ground_3.png", renderer);
    if (!background->layers[6]) goto ERROR_LABEL;

    background->layers[7] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 3/layers/plant.png", renderer);
    if (!background->layers[7]) goto ERROR_LABEL;

    return background;

ERROR_LABEL:
    return NULL;
    
}

Background* load_background_04(RE_Renderer *renderer)
{

    Background* background = calloc(1, sizeof(Background*));
    background->layer_count = 5;
    background->layers = calloc(background->layer_count, sizeof(RE_Texture**));

    background->layers[0] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 4/layers/sky.png", renderer);
    if (!background->layers[0]) goto ERROR_LABEL;

    background->layers[1] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 4/layers/clouds_1.png", renderer);
    if (!background->layers[1]) goto ERROR_LABEL;

    background->layers[2] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 4/layers/clouds_2.png", renderer);
    if (!background->layers[2]) goto ERROR_LABEL;

    background->layers[3] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 4/layers/rocks.png", renderer);
    if (!background->layers[3]) goto ERROR_LABEL;

    background->layers[4] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 4/layers/ground.png", renderer);
    if (!background->layers[4]) goto ERROR_LABEL;

    return background;

ERROR_LABEL:
    return NULL;
    
}

Background* load_background_05(RE_Renderer *renderer)
{

    Background* background = calloc(1, sizeof(Background*));
    background->layer_count = 7;
    background->layers = calloc(background->layer_count, sizeof(RE_Texture**));

    background->layers[0] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 5/layers/layer07_Sky.png", renderer);
    if (!background->layers[0]) goto ERROR_LABEL;

    background->layers[1] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 5/layers/layer06_Rocks.png", renderer);
    if (!background->layers[1]) goto ERROR_LABEL;

    background->layers[2] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 5/layers/layer05_Clouds.png", renderer);
    if (!background->layers[2]) goto ERROR_LABEL;

    background->layers[3] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 5/layers/layer04_Hills_2.png", renderer);
    if (!background->layers[3]) goto ERROR_LABEL;

    background->layers[4] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 5/layers/layer03_Hills_1.png", renderer);
    if (!background->layers[4]) goto ERROR_LABEL;

    background->layers[5] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 5/layers/layer02_Trees.png", renderer);
    if (!background->layers[5]) goto ERROR_LABEL;

    background->layers[6] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 5/layers/layer01_Ground.png", renderer);
    if (!background->layers[6]) goto ERROR_LABEL;

    return background;

    ERROR_LABEL:
        return NULL;
    
}

Background* load_background_06(RE_Renderer *renderer)
{

    Background* background = calloc(1, sizeof(Background*));
    background->layer_count = 7;
    background->layers = calloc(background->layer_count, sizeof(RE_Texture**));

    background->layers[0] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 6/layers/7.png", renderer);
    if (!background->layers[0]) goto ERROR_LABEL;

    background->layers[1] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 6/layers/6.png", renderer);
    if (!background->layers[1]) goto ERROR_LABEL;

    background->layers[2] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 6/layers/5.png", renderer);
    if (!background->layers[2]) goto ERROR_LABEL;

    background->layers[3] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 6/layers/4.png", renderer);
    if (!background->layers[3]) goto ERROR_LABEL;

    background->layers[4] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 6/layers/3.png", renderer);
    if (!background->layers[4]) goto ERROR_LABEL;

    background->layers[5] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 6/layers/2.png", renderer);
    if (!background->layers[5]) goto ERROR_LABEL;

    background->layers[6] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Background 6/layers/1.png", renderer);
    if (!background->layers[6]) goto ERROR_LABEL;

    return background;

    ERROR_LABEL:
        return NULL;
    
}

Background* load_background_default(RE_Renderer *renderer)
{

    Background* background = calloc(1, sizeof(Background*));
    background->layer_count = 5;
    background->layers = calloc(background->layer_count, sizeof(RE_Texture**));

    background->layers[0] = RE_Texture_new(
    "../Assets/Images/Backgrounds/Default/Layer_00.png", renderer);
    if (!background->layers[0]) goto ERROR_LABEL;
    
    background->layers[1] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Default/Layer_01.png", renderer);
    if (!background->layers[1]) goto ERROR_LABEL;
    
    background->layers[2] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Default/Layer_02.png", renderer);
    if (!background->layers[2]) goto ERROR_LABEL;
    
    background->layers[3] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Default/Layer_03.png", renderer);
    if (!background->layers[3]) goto ERROR_LABEL;
    
    background->layers[4] = RE_Texture_new(
        "../Assets/Images/Backgrounds/Default/Layer_04.png", renderer);
    if (!background->layers[4]) goto ERROR_LABEL;

    return background;

    ERROR_LABEL:
        return NULL;
    
}

void Background_free(Background* background)
{
    if(!background) { return; }
    
    for (int i = 0; i < background->layer_count; ++i)
    {
        RE_Texture_free(background->layers[i]);
    }

    //free(background);
    
}

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
        
    textures->bonus = RE_Texture_newSet(
        "../Assets/Images/Map/Bonus.png", renderer, 4, 4);
    if (!textures->bonus) goto ERROR_LABEL;
        
    textures->finish = RE_Texture_new(
        "../Assets/Images/Map/Finish.png", renderer);
    if (!textures->finish) goto ERROR_LABEL;
        
    textures->checkpoint_enabled = RE_Texture_new(
        "../Assets/Images/Map/CheckpointOn.png", renderer);
    if (!textures->checkpoint_enabled) goto ERROR_LABEL;
        
    textures->checkpoint_disabled = RE_Texture_new(
        "../Assets/Images/Map/CheckpointOff.png", renderer);
    if (!textures->checkpoint_disabled) goto ERROR_LABEL;
    
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

    textures->background = RE_Texture_new(
    "../Assets/Images/Backgrounds/Default/Full.png", renderer);
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

    textures->IdlePlayer = RE_Texture_new(
        "../Assets/Images/Player/Idle.png", renderer);
    if (!textures->IdlePlayer) goto ERROR_LABEL;

    textures->IdleFirePlayer = RE_Texture_new(
        "../Assets/Images/Player/IdleFire.png", renderer);
    if (!textures->IdleFirePlayer) goto ERROR_LABEL;

    textures->gravityInverter = RE_Texture_new(
        "../Assets/Images/Map/1.png", renderer);
    if (!textures->gravityInverter) goto ERROR_LABEL;

    textures->fireball = RE_Texture_newSet(
            "../Assets/Images/Player/FireBall.png", renderer,1,6);
    if (!textures->fireball) goto ERROR_LABEL;

    textures->background_default = load_background_default(renderer);
    if (!textures->background_default) goto ERROR_LABEL;
    
    textures->background_01 = load_background_01(renderer);
    if (!textures->background_01) goto ERROR_LABEL;
    
    textures->background_02 = load_background_02(renderer);
    if (!textures->background_02) goto ERROR_LABEL;
    
    textures->background_03 = load_background_03(renderer);
    if (!textures->background_03) goto ERROR_LABEL;

    textures->background_04 = load_background_04(renderer);
    if (!textures->background_04) goto ERROR_LABEL;

    textures->background_05 = load_background_05(renderer);
    if (!textures->background_05) goto ERROR_LABEL;

    textures->background_06 = load_background_06(renderer);
    if (!textures->background_06) goto ERROR_LABEL;

    textures->PowerUP_Scream = RE_Texture_newSet(
        "../Assets/Images/Collectables/PowerUP_Scream.png", renderer, 1, 5);
    if (!textures->PowerUP_Scream) goto ERROR_LABEL;

    textures->scream = RE_Texture_new(
        "../Assets/Images/Player/Scream.png", renderer);
    if (!textures->scream) goto ERROR_LABEL;

    return textures;

ERROR_LABEL:
    printf("ERROR - GameTextures_new()\n");
    GameTextures_free(textures);
    return NULL;
}

Background* Background_get(GameTextures* textures, int index)
{
    switch (index)
    {
        case 1:
            return textures->background_01;
        case 2:
            return textures->background_02;
        case 3:
            return textures->background_03;
        case 4:
            return textures->background_04;
        case 5:
            return textures->background_05;
        case 6:
            return textures->background_06;
        default:
            return textures->background_default;
    }
    
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
    RE_Texture_free(textures->finish);
    RE_Texture_free(textures->checkpoint_disabled);
    RE_Texture_free(textures->checkpoint_enabled);

    Background_free(textures->background_default);
    Background_free(textures->background_01);
    Background_free(textures->background_02);
    Background_free(textures->background_03);
    Background_free(textures->background_04);
    Background_free(textures->background_05);
    Background_free(textures->background_06);
    
    RE_Texture_free(textures->playerRunningFire);
    RE_Texture_free(textures->playerFallingFire);

    RE_Texture_free(textures->PowerUP_Fire);
    RE_Texture_free(textures->RollingPowerUP_Fire);

    RE_Texture_free(textures->IdlePlayer);
    RE_Texture_free(textures->IdleFirePlayer);

    RE_Texture_free(textures->fireball);
    RE_Texture_free(textures->PowerUP_Scream);
    RE_Texture_free(textures->scream);
    free(textures);
}
