#include "Enemy.h"
#include "Enemy_Nut.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

// Méthodes du GameObject
int Enemy_free(GameObject *object);
int Enemy_onStart(GameObject *object);
int Enemy_onRespawn(GameObject *object);
int Enemy_update(GameObject *object);
int Enemy_fixedUpdate(GameObject *object);
int Enemy_render(GameObject *object);

Enemy *Enemy_new(Scene *scene, int type, PE_Vec2 *position)
{
    GameObject *object = NULL;
    Enemy *enemy = NULL;
    int exitStatus = EXIT_SUCCESS;

    object = GameObject_new(scene, GAME_ENEMY);
    if (!object) goto ERROR_LABEL;

    enemy = (Enemy *)calloc(1, sizeof(Enemy));
    if (!enemy) goto ERROR_LABEL;

    enemy->m_object = object;
    enemy->m_type = type;
    enemy->m_startPos = *position;

    // Initialisation de la classe mère
    object->m_data = (void *)enemy;
    object->m_layer = 1;
    object->cm_free = Enemy_free;
    object->cm_onStart = Enemy_onStart;
    object->cm_onRespawn = Enemy_onRespawn;
    object->cm_update = Enemy_update;
    object->cm_fixedUpdate = Enemy_fixedUpdate;
    object->cm_render = Enemy_render;

    // Allocations supplémentaires
    enemy->m_animator = RE_Animator_new();
    if (!enemy->m_animator) goto ERROR_LABEL;

    exitStatus = EXIT_SUCCESS;
    switch (type)
    {
    case ENEMY_NUT:
        exitStatus = Nut_init(enemy);
        break;
    default:
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return enemy;

ERROR_LABEL:
    printf("ERROR - Enemy_new()\n");
    GameObject_free(object);
    return NULL;
}

int Enemy_free(GameObject *object)
{
    Enemy *enemy = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    enemy = GameObject_getEnemy(object);
    if (!enemy) goto ERROR_LABEL;

    RE_Animator_free(enemy->m_animator);

    free(enemy);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Enemy_free()\n");
    return EXIT_FAILURE;
}

int Enemy_onStart(GameObject *object)
{
    Enemy *enemy = NULL;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    enemy = GameObject_getEnemy(object);
    if (!enemy) goto ERROR_LABEL;

    switch (enemy->m_type)
    {
    case ENEMY_NUT:
        exitStatus = Nut_onStart(enemy);
        break;

    default:
        printf("ERROR - Unknown enemy type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Enemy_onStart()\n");
    return EXIT_FAILURE;
}

int Enemy_onRespawn(GameObject *object)
{
    Enemy *enemy = NULL;
    int exitStatus = EXIT_SUCCESS;

    if (!object || !object->m_data) goto ERROR_LABEL;

    enemy = GameObject_getEnemy(object);
    if (!enemy) goto ERROR_LABEL;

    switch (enemy->m_type)
    {
    case ENEMY_NUT:
        exitStatus = Nut_onRespawn(enemy);
        break;

    default:
        printf("ERROR - Unknown enemy type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Enemy_onRespawn()\n");
    return EXIT_FAILURE;
}

int Enemy_update(GameObject *object)
{
    Enemy *enemy = NULL;
    Scene *scene = NULL;
    RE_Timer *time = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    enemy = GameObject_getEnemy(object);
    if (!enemy) goto ERROR_LABEL;

    scene = GameObject_getScene(object);
    time = Scene_getTime(scene);

    RE_Animator_update(enemy->m_animator, time);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Enemy_update()\n");
    return EXIT_FAILURE;
}

int Enemy_fixedUpdate(GameObject *object)
{
    Enemy *enemy = NULL;
    int exitStatus = EXIT_SUCCESS;

    if (!object || !object->m_data) goto ERROR_LABEL;

    enemy = GameObject_getEnemy(object);
    if (!enemy) goto ERROR_LABEL;

    switch (enemy->m_type)
    {
    case ENEMY_NUT:
        exitStatus = Nut_fixedUpdate(enemy);
        break;

    default:
        printf("ERROR - Unknown enemy type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Enemy_fixedUpdate()\n");
    return EXIT_FAILURE;
}

int Enemy_render(GameObject *object)
{
    Enemy *enemy = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    enemy = GameObject_getEnemy(object);
    if (!enemy) goto ERROR_LABEL;

    switch (enemy->m_type)
    {
    case ENEMY_NUT:
        Nut_render(enemy);
        break;

    default:
        break;
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Enemy_render()\n");
    return EXIT_FAILURE;
}

void Enemy_damage(Enemy *enemy)
{
    switch (enemy->m_type)
    {
    case ENEMY_NUT:
        Nut_damage(enemy);
        break;
    default:
        break;
    }
}