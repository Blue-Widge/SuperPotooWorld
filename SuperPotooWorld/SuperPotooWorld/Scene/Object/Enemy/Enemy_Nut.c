#include "Enemy.h"
#include "Enemy_Nut.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

void Nut_onLeftTriggerStay(PE_Trigger *trigger);
void Nut_onLeftTriggerExit(PE_Trigger *trigger);
void Nut_onRightTriggerStay(PE_Trigger *trigger);
void Nut_onRightTriggerExit(PE_Trigger *trigger);
void Nut_onCollisionEnter(PE_Collision* collision);

void Nut_switchGravity(Enemy* enemy)
{

    GameObject *object = Enemy_getObject(enemy);
    RE_Transform transform;

    PE_Body_setGravityScale(GameObject_getBody(object), -Nut_getGravityDirection(enemy));
    RE_Animator_getTransform(enemy->m_animator, &transform);
    
    if (Nut_getGravityDirection(enemy) == INVERTED)
    {
        transform.flipFlags |= RE_FLIP_VERTICAL;
    }
    else
    {
        transform.flipFlags &= ~RE_FLIP_VERTICAL;
    }

    RE_Animator_setTransform(enemy->m_animator, &transform);
    
}

int Nut_init(Enemy *enemy)
{
    Scene *scene = GameObject_getScene(enemy->m_object);
    RE_Animator *animator = enemy->m_animator;
    GameTextures* textures = Scene_getTextures(scene);
    RE_TextureAnim* texAnim = RE_Animator_createTextureAnim(animator, textures->hazelnut, "Hazelnut");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.3f);
    RE_Animator_playTextureAnim(animator, "Hazelnut");

    return EXIT_SUCCESS;

    ERROR_LABEL:
    return EXIT_FAILURE;
}

int Nut_onStart(Enemy *enemy)
{
    Scene *scene = GameObject_getScene(enemy->m_object);
    PE_Vec2 *position = &enemy->m_startPos;
    PE_World *world = NULL;
    PE_Body *body = NULL;
    PE_BodyDef bodyDef;
    PE_Collider *collider = NULL;
    PE_ColliderDef colliderDef;

    // Ajout dans le moteur physique
    world = Scene_getWorld(scene);

    // Création du corps associé
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_DYNAMIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(enemy->m_object, body);

    // Création du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape,0.05f, 0.05f, 0.95f, 0.95f);
    colliderDef.filter.categoryBits = FILTER_ENEMY | FILTER_VISIBLE | FILTER_DAMAGER;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_BLOCK | FILTER_CAMERA | FILTER_DAMAGEABLE | FILTER_DAMAGER;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    int exitStatus = Scene_setToFixedUpdate(scene, enemy->m_object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -5.0, -2.5f, 0.5f, 2.5f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_ENEMY | FILTER_VISIBLE | FILTER_DAMAGER;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_DAMAGEABLE;
    PE_Collider* leftTrigger = PE_Body_createCollider(body, &colliderDef);

    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, 0.5f, -2.5f, 5.0f, 2.5f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_ENEMY | FILTER_VISIBLE | FILTER_DAMAGER;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_DAMAGEABLE;
    PE_Collider* rightTrigger = PE_Body_createCollider(body, &colliderDef);

    PE_Collider_setOnTriggerStay(leftTrigger, Nut_onLeftTriggerStay);
    PE_Collider_setOnTriggerExit(leftTrigger, Nut_onLeftTriggerExit);
    PE_Collider_setOnTriggerStay(rightTrigger, Nut_onRightTriggerStay);
    PE_Collider_setOnTriggerExit(rightTrigger, Nut_onRightTriggerExit);
    
    PE_Collider_setOnCollisionEnter(collider, Nut_onCollisionEnter);

    if(enemy->m_gravityDirection == -1)
    {
        Nut_switchGravity(enemy);
    }

    enemy->m_state = NUT_ASLEEP;
    PE_Body_setPosition(body, &enemy->m_startPos);
    
    return EXIT_SUCCESS;

    ERROR_LABEL:
        printf("ERROR - Nut_onStart()\n");
    return EXIT_FAILURE;
    
}

void Nut_onLeftTriggerStay(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    Enemy* enemy = GameObject_getEnemy(thisObject);

    if(enemy->m_state != NUT_SPINNING)
    { 
        enemy->m_state = NUT_STARTING;
        enemy->direction = LEFT;
    }


}

void Nut_onLeftTriggerExit(PE_Trigger *trigger)
{
    
}

void Nut_onRightTriggerStay(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    Enemy* enemy = GameObject_getEnemy(thisObject);

    if(enemy->m_state != NUT_SPINNING)
    {
        enemy->m_state = NUT_STARTING;
        enemy->direction = RIGHT;
    }
    
}

void Nut_onRightTriggerExit(PE_Trigger *trigger)
{
    
}

int Nut_onRespawn(Enemy *enemy)
{
    // On supprime la noisette du moteur physique si elle y est toujours
    GameObject_removeBody(enemy->m_object);

    // On appelle Nut_onStart() pour recréer son corps dans le moteur physique avec les valeurs initiales
    int exitStatus = Nut_onStart(enemy);
    return exitStatus;
}

int Nut_fixedUpdate(Enemy *enemy)
{
    GameObject* object = enemy->m_object;
    Scene *scene = NULL;
    RE_Timer *time = NULL;
    GameInput *input = NULL;
    PE_Body *body = NULL;
    PE_Vec2 velocity;

    if (!object || !object->m_data) goto ERROR_LABEL;

    scene = GameObject_getScene(object);
    body = GameObject_getBody(object);
    time = Scene_getTime(scene);
    input = Scene_getInput(scene);

    if(enemy->m_state == NUT_STARTING)
    {
        enemy->m_state = NUT_SPINNING;
    }
    if(enemy->m_state == NUT_SPINNING)
    {
        PE_Body_getVelocity(body, &velocity);
        velocity.x = enemy->direction * 8.f;
        PE_Body_setVelocity(body, &velocity);
    }

    return EXIT_SUCCESS;

    ERROR_LABEL:
    printf("ERROR - Enemy_fixedUpdate()\n");
    return EXIT_FAILURE;
    
}

void Nut_onCollisionEnter(PE_Collision* collision)
{
    PE_Body *thisBody = PE_Collision_getBody(collision);
    PE_Body *otherBody = PE_Collision_getOtherBody(collision);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    GameObject *otherObject = PE_Body_getUserData(otherBody);
    
    if (GameObject_getType(otherObject) == GAME_BLOCK || GameObject_getType(otherObject) == GAME_ENEMY)
    {
        int relPos = PE_Collision_getRelativePosition(collision);
        PE_Vec2 velocity;
        PE_Body_getVelocity(thisBody, &velocity);
        Enemy* enemy = GameObject_getEnemy(thisObject);

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
            enemy->direction *= -1;
            break;

        default:
            break;
        }
        PE_Body_setCollisionResponse(thisBody, &velocity);
    }
}


void Nut_render(Enemy *enemy)
{

    Camera *camera = Scene_getCamera(enemy->m_object->m_scene);
    float viewX, viewY;
    PE_Vec2 position;
    PE_Body* body = GameObject_getBody(enemy->m_object);
    position.x = body->m_position.x;
    position.y = body->m_position.y + 1;
    Camera_worldToView(camera, &position, &viewX, &viewY);
    GameTextures* textures = Scene_getTextures(GameObject_getScene(enemy->m_object));
    if(enemy->m_state == NUT_ASLEEP)
    {
        if(Nut_getGravityDirection(enemy) == INVERTED)
        {
            RE_Texture_renderFlipF(textures->hazelnut, 0, viewX, viewY, RE_FLIP_VERTICAL);
        }
        else
        {
            RE_Texture_renderF(textures->hazelnut, 0, viewX, viewY);
        }
    }
    else
    {
        RE_Animator_renderF(enemy->m_animator, viewX, viewY);
    }

}

void Nut_damage(Enemy *enemy)
{
    Scene_removeObject(GameObject_getScene(Enemy_getObject(enemy)), Enemy_getObject(enemy));
}
