#include "Collectable.h"
#include "Collectable_Firefly.h"
#include "Collectable_Heart.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

// Fonctions du GameObject
int Collectable_free(GameObject *object);
int Collectable_onStart(GameObject *object);
int Collectable_onRespawn(GameObject *object);
int Collectable_render(GameObject *object);

Collectable *Collectable_new(Scene *scene, int type, PE_Vec2 *position)
{
    GameObject *object = NULL;
    Collectable *collectable = NULL;

    object = GameObject_new(scene, GAME_COLLECTABLE);
    if (!object) goto ERROR_LABEL;

    collectable = (Collectable *)calloc(1, sizeof(Collectable));
    if (!collectable) goto ERROR_LABEL;

    collectable->m_object = object;
    collectable->m_type = type;
    collectable->m_startPos = *position;

    // Initialisation de la classe mère
    object->m_data = (void *)collectable;
    object->m_layer = 2;
    object->cm_free = Collectable_free;
    object->cm_onStart = Collectable_onStart;
    object->cm_onRespawn = Collectable_onRespawn;
    object->cm_render = Collectable_render;

    return collectable;

ERROR_LABEL:
    printf("ERROR - Collectable_new()\n");
    GameObject_free(object);
    return NULL;
}

int Collectable_free(GameObject *object)
{
    Collectable *collectable = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    collectable = GameObject_getCollectable(object);
    if (!collectable) goto ERROR_LABEL;

    free(collectable);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Collectable_free()\n");
    return EXIT_FAILURE;
}

int Collectable_onStart(GameObject *object)
{
    Collectable *collectable = NULL;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    collectable = GameObject_getCollectable(object);
    if (!collectable) goto ERROR_LABEL;

    switch (collectable->m_type)
    {
    case COLLECTABLE_FIREFLY:
        exitStatus = Firefly_onStart(collectable);
        break;

    case COLLECTABLE_HEART:
        exitStatus = Heart_onStart(collectable);
        break;

    default:
        printf("ERROR - Unknown collectable type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Collectable_onStart()\n");
    return EXIT_FAILURE;
}

int Collectable_onRespawn(GameObject *object)
{
    Collectable *collectable = NULL;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    collectable = GameObject_getCollectable(object);
    if (!collectable) goto ERROR_LABEL;

    switch (collectable->m_type)
    {
    case COLLECTABLE_FIREFLY:
        exitStatus = Firefly_onRespawn(collectable);
        break;

    case COLLECTABLE_HEART:
        exitStatus = Heart_onRespawn(collectable);
        break;

    default:
        printf("ERROR - Unknown collectable type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Collectable_onRespawn()\n");
    return EXIT_FAILURE;
}

int Collectable_render(GameObject *object)
{
    Collectable *collectable = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    collectable = GameObject_getCollectable(object);
    if (!collectable) goto ERROR_LABEL;

    switch (collectable->m_type)
    {
    case COLLECTABLE_FIREFLY:
        Firefly_render(collectable);
        break;

    case COLLECTABLE_HEART:
        Heart_render(collectable);
        break;

    default:
        break;
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Collectable_onRespawn()\n");
    return EXIT_FAILURE;
}
