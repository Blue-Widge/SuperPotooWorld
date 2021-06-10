#include "Block.h"
#include "Block_GravityInverter.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

void GravityInverter_onTriggerEnter(PE_Trigger *trigger)
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
        Player_switchGravity(player);
    }
}

int GravityInverter_onStart(Block *block)
{
    Scene *scene = GameObject_getScene(block->m_object);
    PE_Vec2 *position = &block->m_startPos;
    PE_AABB *aabb = &block->m_startAABB;
    PE_World *world = NULL;
    PE_Body *body = NULL;
    PE_BodyDef bodyDef;
    PE_Collider *collider = NULL;
    PE_ColliderDef colliderDef;

    // Ajout dans le moteur physique
    world = Scene_getWorld(scene);

    // Cr�ation du corps associ�
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(block->m_object, body);

    // Cr�ation du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape,
                      aabb->lower.x, aabb->lower.y, aabb->upper.x, aabb->upper.y);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_BLOCK | FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_GRAVITY | FILTER_CAMERA;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    PE_Collider_setOnTriggerEnter(collider, GravityInverter_onTriggerEnter);
    return EXIT_SUCCESS;

    ERROR_LABEL:
        printf("ERROR - Bonus_onStart()\n");
    return EXIT_FAILURE;
}

int GravityInverter_onRespawn(Block *block)
{
    return EXIT_SUCCESS;
}

void GravityInverter_render(Block *block)
{
    Camera *camera = Scene_getCamera(block->m_object->m_scene);
    GameTextures *textures = Scene_getTextures(block->m_object->m_scene);
    float viewX, viewY;
    PE_Vec2 position;
    position.x = block->m_startPos.x;
    position.y = block->m_startPos.y + 1;
    Camera_worldToView(camera, &position, &viewX, &viewY);

    RE_Texture_render(textures->gravityInverter, 0, (int)viewX, (int)viewY);

    
}
