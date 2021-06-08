#include "Collectable.h"
#include "Collectable_Heart.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

void Heart_onTriggerEnter(PE_Trigger *trigger)
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
        Scene_disableObject(scene, thisObject);
        Player_addHeart(player);
    }
}

int Heart_onStart(Collectable *collectable)
{
    Scene *scene = GameObject_getScene(collectable->m_object);
    PE_Vec2 *position = &collectable->m_startPos;
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

    GameObject_setBody(collectable->m_object, body);

    // Création du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, 0.0f, 0.0f, 1.0f, 1.0f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_COLLECTABLE | FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_CAMERA;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    // Callback du collider
    PE_Collider_setOnTriggerEnter(collider, Heart_onTriggerEnter);
    
    return EXIT_SUCCESS;

    ERROR_LABEL:
        printf("ERROR - Firefly_onStart()\n");
    return EXIT_FAILURE;

}

int Heart_onRespawn(Collectable *collectable)
{
    return EXIT_SUCCESS;
}

void Heart_render(Collectable *collectable)
{

    Camera *camera = Scene_getCamera(collectable->m_object->m_scene);
    GameTextures *textures = Scene_getTextures(collectable->m_object->m_scene);

    float viewX, viewY;
    PE_Vec2 position;
    position.x = collectable->m_startPos.x;
    position.y = collectable->m_startPos.y + 1;
    Camera_worldToView(camera, &position, &viewX, &viewY);
    
    RE_Texture_render(textures->heart, 0, (int)viewX, (int)viewY);
    
}
