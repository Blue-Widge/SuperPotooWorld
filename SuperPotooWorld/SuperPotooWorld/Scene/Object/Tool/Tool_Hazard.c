#include "Tool.h"
#include "Tool_Hazard.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

// Fonction permettant de tuer le joueur quand il tombe
// Il entre alors en collision avec le Hazard qui appelle Player_kill()
void Hazard_onTriggerEnter(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    PE_Body *otherBody = PE_Trigger_getOtherBody(trigger);
    GameObject *thisObject = (GameObject *)PE_Body_getUserData(thisBody);
    GameObject *otherObject = (GameObject *)PE_Body_getUserData(otherBody);
    Player *player = NULL;

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Hazard_onTriggerEnter()\n");
        return;
    }

    player = GameObject_getPlayer(otherObject);
    if (player)
    {
        Player_kill(player);
    }
}

int Hazard_onStart(Tool *tool)
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

    // Création du corps associé
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(tool->m_object, body);

    // Création du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape,
                      aabb->lower.x, aabb->lower.y, aabb->upper.x, aabb->upper.y);
    colliderDef.filter.categoryBits = FILTER_TOOL;
    colliderDef.filter.maskBits = FILTER_COLLECTABLE | FILTER_ENEMY | FILTER_PLAYER;
    colliderDef.isTrigger = TRUE;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    // Callback
    PE_Collider_setOnTriggerEnter(collider, Hazard_onTriggerEnter);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Hazard_onStart()\n");
    return EXIT_FAILURE;
}
