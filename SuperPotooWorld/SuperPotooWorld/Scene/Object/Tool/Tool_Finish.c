#include "Tool.h"
#include "Tool_Finish.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

void Finish_onTriggerEnter(PE_Trigger *trigger)
{

    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    PE_Body *otherBody = PE_Trigger_getOtherBody(trigger);
    GameObject *thisObject = (GameObject *)PE_Body_getUserData(thisBody);
    GameObject *otherObject = (GameObject *)PE_Body_getUserData(otherBody);
    Player *player = NULL;

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Heart_onTriggerEnter()\n");
        return;
    }

    player = GameObject_getPlayer(otherObject);
    if (player)
    {
        Scene *scene = GameObject_getScene(thisObject);
        Scene_finish(scene);
    }
    
}

int Finish_onStart(Tool *tool)
{
    
    Scene *scene = GameObject_getScene(tool->m_object);
    PE_Vec2 *position = &tool->m_startPos;
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
    PE_Shape_setAsBox(&colliderDef.shape,0.0, 0.0, 1.0f, 1.0f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_TOOL | FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_CAMERA;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;
    PE_Collider_setOnTriggerEnter(collider, Finish_onTriggerEnter);

    return EXIT_SUCCESS;

    ERROR_LABEL:
        printf("ERROR - Finish_onStart()\n");
        return EXIT_FAILURE;
    
}

void Finish_render(Tool* tool)
{
    
    Camera *camera = Scene_getCamera(tool->m_object->m_scene);
    GameTextures *textures = Scene_getTextures(tool->m_object->m_scene);

    float viewX, viewY;
    PE_Vec2 position;
    position.x = tool->m_startPos.x;
    position.y = tool->m_startPos.y + 1;
    Camera_worldToView(camera, &position, &viewX, &viewY);
    
    RE_Texture_render(textures->finish, 0, (int)viewX, (int)viewY);
    
}