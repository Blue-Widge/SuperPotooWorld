#include "Tool.h"
#include "Tool_Bound.h"
#include "Tool_Checkpoint.h"
#include "Tool_Finish.h"
#include "Tool_Hazard.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

// Méthodes du GameObject
int Tool_free(GameObject *object);
int Tool_onStart(GameObject *object);
//int Tool_onRespawn(GameObject *object);
int Tool_render(GameObject *object);

Tool *Tool_new(Scene *scene, int type, PE_Vec2 *position, PE_AABB *aabb)
{
    GameObject *object = NULL;
    Tool *tool = NULL;

    object = GameObject_new(scene, GAME_TOOL);
    if (!object) goto ERROR_LABEL;

    tool = (Tool *)calloc(1, sizeof(Tool));
    if (!tool) goto ERROR_LABEL;

    tool->m_object = object;
    tool->m_type = type;
    tool->m_startPos = *position;
    tool->m_startAABB = *aabb;

    // Initialisation de la classe mère
    object->m_data = (void *)tool;
    object->m_layer = 0;
    object->cm_free = Tool_free;
    object->cm_onStart = Tool_onStart;
    //object->cm_onRespawn = Tool_onRespawn;
    object->cm_render = Tool_render;

    return tool;

ERROR_LABEL:
    printf("ERROR - Tool_new()\n");
    GameObject_free(object);
    return NULL;
}

int Tool_free(GameObject *object)
{
    Tool *tool = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    tool = GameObject_getTool(object);
    if (!tool) goto ERROR_LABEL;

    free(tool);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Tool_free()\n");
    return EXIT_FAILURE;
}

int Tool_onStart(GameObject *object)
{
    Tool *tool = NULL;
    int exitStatus = EXIT_SUCCESS;

    if (!object || !object->m_data) goto ERROR_LABEL;

    tool = GameObject_getTool(object);
    if (!tool) goto ERROR_LABEL;

    exitStatus = EXIT_SUCCESS;
    switch (tool->m_type)
    {
    case TOOL_BOUND:
        exitStatus = Bound_onStart(tool);
        break;

    case TOOL_CHECKPOINT:
        exitStatus = Checkpoint_onStart(tool);
        break;

    case TOOL_FINISH:
        exitStatus = Finish_onStart(tool);
        break;

    case TOOL_HAZARD:
        exitStatus = Hazard_onStart(tool);
        break;

    default:
        printf("ERROR - Unknown tool type\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Tool_onStart()\n");
    return EXIT_FAILURE;
}

/*int Tool_onRespawn(GameObject *object)
{
    Tool *tool = NULL;
    int exitStatus = EXIT_SUCCESS;

    if (!object || !object->m_data) goto ERROR_LABEL;

    tool = GameObject_getTool(object);
    if (!tool) goto ERROR_LABEL;

    switch (tool->m_type)
    {
    default:
        printf("ERROR - This tool cannot be respawn\n");
        goto ERROR_LABEL;
        break;
    }
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Tool_onRespawn()\n");
    return EXIT_FAILURE;
}*/

int Tool_render(GameObject *object)
{
    Tool *tool = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    tool = GameObject_getTool(object);
    if (!tool) goto ERROR_LABEL;

    switch (tool->m_type)
    {
    case TOOL_CHECKPOINT:
        Checkpoint_render(tool);
        break;

    default:
        break;
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Tool_render()\n");
    return EXIT_FAILURE;
}