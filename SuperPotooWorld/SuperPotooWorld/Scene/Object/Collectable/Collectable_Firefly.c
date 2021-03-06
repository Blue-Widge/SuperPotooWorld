#include "Collectable.h"
#include "Collectable_Firefly.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

// Fonction de rappel
// Cette fonction est appel?e quand le joueur entre en collision avec la luciole
// Elle est affect?e au collider (trigger) de la luciole.

void Firefly_onTriggerEnter(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    PE_Body *otherBody = PE_Trigger_getOtherBody(trigger);
    GameObject *thisObject = (GameObject *)PE_Body_getUserData(thisBody);
    GameObject *otherObject = (GameObject *)PE_Body_getUserData(otherBody);
    Player *player = NULL;

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Firefly_onTriggerEnter()\n");
        return;
    }

    player = GameObject_getPlayer(otherObject);
    if (player)
    {
        Scene *scene = GameObject_getScene(thisObject);
        Scene_disableObject(scene, thisObject);
        Player_addFirefly(player);
    }
}

int Firefly_onStart(Collectable *collectable)
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

    // Cr?ation du corps associ?
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(collectable->m_object, body);

    // Cr?ation du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape,0.0, 0.0, 1.0f, 1.0f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_COLLECTABLE | FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_CAMERA;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    // Callback du collider
    PE_Collider_setOnTriggerEnter(collider, Firefly_onTriggerEnter);

    RE_Animator* animator = Scene_getAnimators(scene)->firefly;
    RE_Animator_playTextureAnim(animator, "Firefly");


    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Firefly_onStart()\n");
    return EXIT_FAILURE;
}

int Firefly_onRespawn(Collectable *collectable)
{
    int exitStatus = Firefly_onStart(collectable);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Firefly_onRespawn()\n");
    return EXIT_FAILURE;
}

void Firefly_render(Collectable *collectable)
{
    Scene *scene = GameObject_getScene(collectable->m_object);
    GameTextures *textures = Scene_getTextures(scene);
    GameAnimators *animators = Scene_getAnimators(scene);
    PE_Vec2 position = GameObject_getPosition(collectable->m_object);
    Camera *camera = Scene_getCamera(scene);

    float x, y;
    position.x = collectable->m_startPos.x;
    position.y = collectable->m_startPos.y + 1;
    Camera_worldToView(camera, &position, &x, &y);
    RE_Animator_renderF(animators->firefly, x, y);

}
