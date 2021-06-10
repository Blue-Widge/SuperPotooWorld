#include "GameAnimators.h"
#include "Scene.h"

#include <stdio.h>
#include <stdlib.h>

GameAnimators *GameAnimators_new(Scene *scene)
{
    GameAnimators *animators = NULL;
    int exitStatus = EXIT_SUCCESS;
    RE_ParamAnim* param = NULL;

    // Vérifie que les textures sont chargées
    if (!Scene_getTextures(scene)) goto ERROR_LABEL;

    animators = (GameAnimators *)calloc(1, sizeof(GameAnimators));
    if (!animators) goto ERROR_LABEL;

    animators->m_scene = scene;

    animators->firefly = RE_Animator_new();
    if (!animators->firefly) goto ERROR_LABEL;

    animators->bonus = RE_Animator_new();
    if (!animators->bonus) goto ERROR_LABEL;
    
    animators->RollingPowerUP_Fire = RE_Animator_new();
    if (!animators->RollingPowerUP_Fire) goto ERROR_LABEL;
    
    animators->fireball = RE_Animator_new();
    if (!animators->fireball) goto ERROR_LABEL;

    GameTextures* textures = Scene_getTextures(scene);
    
    RE_TextureAnim* texAnim = RE_Animator_createTextureAnim(animators->bonus, textures->bonus, "Bonus");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animators->firefly, textures->firefly, "Firefly");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animators->RollingPowerUP_Fire, textures->PowerUP_Fire, "RollingPowerUP_Fire");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animators->fireball, textures->fireball, "Fireball");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.3f);

    /*
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
    exitStatus = RE_Animator_playTextureAnim(animator, "RollingPowerUP_Fire");*/

    return animators;

ERROR_LABEL:
    printf("ERROR - GameAnimators_new()\n");
    GameAnimators_free(animators);
    return NULL;
}

void GameAnimators_free(GameAnimators *animators)
{
    if (!animators)
        return;

    RE_Animator_free(animators->firefly);
    RE_Animator_free(animators->bonus);
    RE_Animator_free(animators->fireball);

    //AJOUT
    RE_Animator_free(animators->RollingPowerUP_Fire);

    free(animators);
}

void GameAnimators_update(GameAnimators *animators)
{
    RE_Timer *time = Scene_getTime(animators->m_scene);
    RE_Animator_update(animators->firefly, time);
    RE_Animator_update(animators->bonus, time);
    RE_Animator_update(animators->fireball, time);

    //AJOUT
    RE_Animator_update(animators->RollingPowerUP_Fire, time);
}
