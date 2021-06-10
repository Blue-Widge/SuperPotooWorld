#include "Skill.h"
#include "FireBall.h"
#include "../../Scene.h"
#include <stdio.h>
#include <stdlib.h>

// Fonctions du GameObject
int Skill_free(GameObject* object);
int Skill_onStart(GameObject* object);
int Skill_onRespawn(GameObject* object);
int Skill_render(GameObject* object);

Skill* Skill_new(Scene* scene, int type, PE_Vec2* position)
{
    GameObject* object = NULL;
    Skill* skill = NULL;

    object = GameObject_new(scene, GAME_SKILL);
    if (!object) goto ERROR_LABEL;

    skill = (Skill*)calloc(1, sizeof(Skill));
    if (!skill) goto ERROR_LABEL;

    skill->m_object = object;
    skill->m_type = type;
    skill->m_startPos = *position;

    // Initialisation de la classe m�re
    object->m_data = (void*)skill;
    object->m_layer = 2;
    object->cm_free = Skill_free;
    object->cm_onStart = Skill_onStart;
    object->cm_onRespawn = Skill_onRespawn;
    object->cm_render = Skill_render;

    return skill;

ERROR_LABEL:
    printf("ERROR - Skill_new()\n");
    GameObject_free(object);
    return NULL;
}

int Skill_free(GameObject* object)
{
    Skill* skill = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    skill = GameObject_getSkill(object);
    if (!skill) goto ERROR_LABEL;

    free(skill);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Skill_free()\n");
    return EXIT_FAILURE;
}

int Skill_onStart(GameObject* object)
{
    Skill* skill = NULL;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    skill = GameObject_getSkill(object);
    if (!skill) goto ERROR_LABEL;

    switch (skill->m_type)
    {
    case FIREBALL:
        exitStatus = FireBall_onStart(skill);
        break;

    default:
        printf("ERROR - Unknown skill type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Skill_onStart()\n");
    return EXIT_FAILURE;
}

int Skill_onRespawn(GameObject* object)
{
    Skill* skill = NULL;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    skill = GameObject_getSkill(object);
    if (!skill) goto ERROR_LABEL;

    switch (skill->m_type)
    {
    case FIREBALL:
        exitStatus = FireBall_onRespawn(skill);
        break;
    default:
        printf("ERROR - Unknown skill type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Skill_onRespawn()\n");
    return EXIT_FAILURE;
}

int Skill_render(GameObject* object)
{
    Skill* skill = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    skill = GameObject_getSkill(object);
    if (!skill) goto ERROR_LABEL;

    switch (skill->m_type)
    {
    case FIREBALL:
        FireBall_render(skill);
        break;
    default:
        break;
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Skill_render()\n");
    return EXIT_FAILURE;
}