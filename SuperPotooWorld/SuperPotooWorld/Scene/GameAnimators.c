#include "GameAnimators.h"
#include "Scene.h"

#include <stdio.h>
#include <stdlib.h>

GameAnimators *GameAnimators_new(Scene *scene)
{
    GameAnimators *animators = NULL;
    int exitStatus = EXIT_SUCCESS;

    // V�rifie que les textures sont charg�es
    if (!Scene_getTextures(scene)) goto ERROR_LABEL;

    animators = (GameAnimators *)calloc(1, sizeof(GameAnimators));
    if (!animators) goto ERROR_LABEL;

    animators->m_scene = scene;

    animators->firefly = RE_Animator_new();
    if (!animators->firefly) goto ERROR_LABEL;

    animators->bonus = RE_Animator_new();
    if (!animators->bonus) goto ERROR_LABEL;

    // TODO
    // Cr�ez les animations des lucioles et bonus
    // Initialisez les animateurs

    GameTextures* textures = Scene_getTextures(scene);
    RE_TextureAnim* texAnim = RE_Animator_createTextureAnim(animators->bonus, textures->bonus, "Bonus");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

    texAnim = RE_Animator_createTextureAnim(animators->firefly, textures->firefly, "Firefly");
    if (!texAnim) goto ERROR_LABEL;

    RE_TextureAnim_setCycleTime(texAnim, 0.2f);

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

    free(animators);
}

void GameAnimators_update(GameAnimators *animators)
{
    RE_Timer *time = Scene_getTime(animators->m_scene);
    RE_Animator_update(animators->firefly, time);
    RE_Animator_update(animators->bonus, time);
}
