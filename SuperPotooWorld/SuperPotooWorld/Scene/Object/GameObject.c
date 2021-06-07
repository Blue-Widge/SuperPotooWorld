#include "GameObject.h"
#include "../Scene.h"

#include <stdio.h>
#include <stdlib.h>

GameObject *GameObject_new(Scene *scene, int type)
{
    GameObject *object = NULL;

    object = (GameObject *)calloc(1, sizeof(GameObject));
    if (!object) goto ERROR_LABEL;

    object->m_scene = scene;
    object->m_type = type;
    object->m_data = NULL;
    object->m_body = NULL;
    object->m_flags = 0;
    object->m_layer = 0;

    object->cm_onStart = NULL;
    object->cm_onRespawn = NULL;
    object->cm_update = NULL;
    object->cm_fixedUpdate = NULL;
    object->cm_render = NULL;
    object->cm_free = NULL;

    return object;

ERROR_LABEL:
    printf("ERROR - GameObject_new()\n");
    return NULL;
}

void GameObject_free(GameObject *object)
{
    if (!object)
        return;

    if (object->cm_free && object->m_data)
    {
        int exitStatus = (object->cm_free)(object);
        if (exitStatus != EXIT_SUCCESS)
            printf("ERROR - GameObject_free()\n");
    }

    GameObject_removeBody(object);

    free(object);
}

void GameObject_removeBody(GameObject *object)
{
    if (!object->m_body)
        return;

    PE_Body *body = object->m_body;
    PE_World *world = PE_Body_getWorld(body);
    PE_World_removeBody(world, body);
    object->m_body = NULL;
}

PE_Vec2 GameObject_getPosition(GameObject *object)
{
    PE_Vec2 position;

    if (!object->m_body)
    {
        printf("ERROR - GameObject_getPosition()\n");
        printf("      - No body is attached to this object\n");
        PE_Vec2_set(&position, 0.f, 0.f);
        return position;
    }

    float alpha = Scene_getAlpha(object->m_scene);
    PE_Body_getInterpolation(object->m_body, alpha, &position);

    return position;
}

PE_Vec2 GameObject_getVelocity(GameObject *object)
{
    PE_Vec2 velocity;

    if (!object->m_body)
    {
        printf("ERROR - GameObject_getVelocity()\n");
        printf("      - No body is attached to this object\n");
        PE_Vec2_set(&velocity, 0.f, 0.f);
        return velocity;
    }

    PE_Body_getVelocity(object->m_body, &velocity);

    return velocity;
}

int GameObject_onRespawn(GameObject *object)
{
    if (!(object->cm_onRespawn && object->m_data))
    {
        printf("ERROR - This object has no onRespawn() method\n");
        goto ERROR_LABEL;
    }

    int exitStatus = (object->cm_onRespawn)(object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - GameObject_onRespawn()\n");
    return EXIT_FAILURE;
}

int GameObject_onStart(GameObject *object)
{
    if (!(object->cm_onStart && object->m_data))
    {
        printf("ERROR - This object has no onStart() method\n");
        goto ERROR_LABEL;
    }

    int exitStatus = (object->cm_onStart)(object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - GameObject_onStart()\n");
    return EXIT_FAILURE;
}

int GameObject_update(GameObject *object)
{
    if (!(object->cm_update && object->m_data))
    {
        printf("ERROR - This object has no update() method\n");
        goto ERROR_LABEL;
    }

    int exitStatus = (object->cm_update)(object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - GameObject_update()\n");
    return EXIT_FAILURE;
}

int GameObject_fixedUpdate(GameObject *object)
{
    if (!(object->cm_fixedUpdate && object->m_data))
    {
        printf("ERROR - This object has no fixedUpdate() method\n");
        goto ERROR_LABEL;
    }

    int exitStatus = (object->cm_fixedUpdate)(object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - GameObject_fixedUpdate()\n");
    return EXIT_FAILURE;
}

int GameObject_render(GameObject *object)
{
    if (!(object->cm_render && object->m_data))
    {
        printf("ERROR - This object has no render() method\n");
        goto ERROR_LABEL;
    }

    int exitStatus = (object->cm_render)(object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - GameObject_render()\n");
    return EXIT_FAILURE;
}
