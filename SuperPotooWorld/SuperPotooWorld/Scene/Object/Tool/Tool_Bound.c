#include "Tool.h"
#include "Tool_Hazard.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int Bound_onStart(Tool *tool)
{
    Scene *scene = GameObject_getScene(tool->m_object);
    PE_Vec2 *position = &tool->m_startPos;
    PE_AABB *aabb = &tool->m_startAABB;
    PE_World *world = NULL;
    PE_Body *body = NULL;
    PE_BodyDef bodyDef;
    PE_Collider *collider = NULL;
    PE_ColliderDef colliderDef;

    // Ajout dans le moteur physique
    world = Scene_getWorld(scene);

    // Cr?ation du corps associ?
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(tool->m_object, body);

    // Cr?ation du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape,
                      aabb->lower.x, aabb->lower.y, aabb->upper.x, aabb->upper.y);
    colliderDef.filter.categoryBits = FILTER_TOOL;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Bound_onStart()\n");
    return EXIT_FAILURE;
}
