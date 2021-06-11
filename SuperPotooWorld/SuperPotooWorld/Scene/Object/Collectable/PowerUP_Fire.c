
#include "PowerUP_Fire.h"
#include "Collectable.h"


#include "../../Scene.h"
#include <stdio.h>
#include <stdlib.h>

void PowerUP_Fire_onCollisionEnter(PE_Collision* collision);

int PowerUP_Fire_fixedUpdate(GameObject *object)
{
    PE_Vec2 velocity; 
    PE_Body_getVelocity(GameObject_getBody(object), &velocity);
    velocity.x = 3.0f;
    PE_Body_setVelocity(GameObject_getBody(object), &velocity);

    return EXIT_SUCCESS;
}

void PowerUP_Fire_onCollisionEnter(PE_Collision* collision)
{
    PE_Body *thisBody = PE_Collision_getBody(collision);
    PE_Body *otherBody = PE_Collision_getOtherBody(collision);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    GameObject *otherObject = PE_Body_getUserData(otherBody);

    if(GameObject_getType(otherObject) == GAME_PLAYER)
    {
        Player* player = GameObject_getPlayer(otherObject);
        if (player)
        {
            Scene* scene = GameObject_getScene(thisObject);
            Scene_disableObject(scene, thisObject);
            Player_powerUp(player, POWERUP_FIRE);
        }
    }
    
    if (GameObject_getType(otherObject) == GAME_BLOCK)
    {
        int relPos = PE_Collision_getRelativePosition(collision);
        PE_Vec2 velocity;
        PE_Body_getVelocity(thisBody, &velocity);

        switch (relPos)
        {
        case PE_ABOVE:
            if (velocity.y < 0.f)
                velocity.y = 0.f;
            break;

        case PE_BELOW:
            if (velocity.y > 0.f)
                velocity.y = 0.f;
            break;

        case PE_RIGHT:
        case PE_LEFT:
            velocity.x = 0.f;
            break;

        default:
            break;
        }
        PE_Body_setCollisionResponse(thisBody, &velocity);
    }
}

int PowerUP_Fire_onStart(Collectable* collectable)
{
    Scene* scene = GameObject_getScene(collectable->m_object);
    PE_Vec2* position = &collectable->m_startPos;
    PE_World* world = NULL;
    PE_Body* body = NULL;
    PE_BodyDef bodyDef;
    PE_Collider* collider = NULL;
    PE_ColliderDef colliderDef;

    GameObject* object = Collectable_getObject(collectable);

    // Ajout dans le moteur physique
    world = Scene_getWorld(scene);

    // Création du corps associé
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_DYNAMIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(collectable->m_object, body);

    // Création du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, 0.0, 0.0, 1.0f, 1.0f);
    colliderDef.filter.categoryBits = FILTER_COLLECTABLE | FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_CAMERA | FILTER_BLOCK;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    // Callback du collider
   // PE_Collider_setOnTriggerEnter(collider, PowerUP_Fire_onTriggerEnter);
    PE_Collider_setOnCollisionEnter(collider, PowerUP_Fire_onCollisionEnter);

    RE_Animator* animator = Scene_getAnimators(scene)->RollingPowerUP_Fire;
    RE_Animator_playTextureAnim(animator, "RollingPowerUP_Fire");

    PE_Vec2 velocity = GameObject_getVelocity(Collectable_getObject(collectable));
    velocity.y = 10.0f;
    PE_Body_setVelocity(GameObject_getBody(Collectable_getObject(collectable)), &velocity);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - PowerUP_Fire_onStart()\n");
    return EXIT_FAILURE;
}

int PowerUP_Fire_onRespawn(Collectable* collectable, int type)
{
    return EXIT_SUCCESS;
}

void PowerUP_Fire_render(Collectable* collectable, int type)
{
    Scene* scene = GameObject_getScene(collectable->m_object);
    GameTextures* textures = Scene_getTextures(scene);
    GameAnimators* animators = Scene_getAnimators(scene);
    PE_Vec2 position = GameObject_getPosition(collectable->m_object);
    Camera* camera = Scene_getCamera(scene);

    float x, y;
    position.x = GameObject_getPosition(collectable->m_object).x;
    position.y = GameObject_getPosition(collectable->m_object).y + 1;
    Camera_worldToView(camera, &position, &x, &y);
    RE_Animator_renderF(animators->RollingPowerUP_Fire, x, y);

}

