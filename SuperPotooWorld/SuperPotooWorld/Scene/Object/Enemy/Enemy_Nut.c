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
    PE_Shape_setAsBox(&colliderDef.shape,0.0, 0.0, 1.0f, 1.0f);
    colliderDef.filter.categoryBits = FILTER_ENEMY | FILTER_VISIBLE | FILTER_DAMAGER;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_BLOCK | FILTER_CAMERA | FILTER_DAMAGEABLE;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    int exitStatus = Scene_setToFixedUpdate(scene, enemy->m_object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, -8.0f, 0.0f, 0.5f, 10.0f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_ENEMY | FILTER_VISIBLE | FILTER_DAMAGER;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_DAMAGEABLE;
    PE_Collider* leftTrigger = PE_Body_createCollider(body, &colliderDef);

    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, 0.5f, 0.0f, 8.0f, 10.0f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_ENEMY | FILTER_VISIBLE | FILTER_DAMAGER;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_DAMAGEABLE;
    PE_Collider* rightTrigger = PE_Body_createCollider(body, &colliderDef);

    PE_Collider_setOnTriggerStay(leftTrigger, Nut_onLeftTriggerStay);
    PE_Collider_setOnTriggerExit(leftTrigger, Nut_onLeftTriggerExit);
    PE_Collider_setOnTriggerStay(rightTrigger, Nut_onRightTriggerStay);
    PE_Collider_setOnTriggerExit(rightTrigger, Nut_onRightTriggerExit);
    
    PE_Collider_setOnCollisionEnter(collider, Nut_onCollisionEnter);

    printf("Nut_onStart : Initialisation d'une noisette\n");

    return EXIT_SUCCESS;

    ERROR_LABEL:
        printf("ERROR - Player_onStart()\n");
    return EXIT_FAILURE;
    
}

void Nut_onLeftTriggerStay(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    Enemy* enemy = GameObject_getEnemy(thisObject);

    enemy->m_state = NUT_STARTING;
    enemy->direction = LEFT;

}

void Nut_onLeftTriggerExit(PE_Trigger *trigger)
{
    
}

void Nut_onRightTriggerStay(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    GameObject *thisObject = PE_Body_getUserData(thisBody);
    Enemy* enemy = GameObject_getEnemy(thisObject);

    enemy->m_state = NUT_STARTING;
    enemy->direction = RIGHT;
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
    // TODO
    // Utilisez cette fonction pour modifier la physique d'une noisette.
    // Vous pouvez la mettre en mouvement quand elle est dans l'état NUT_STARTING.
    // Cet état signifie qu'elle a vu le joueur mais qu'elle ne tourne pas encore.

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
        PE_Body_getVelocity(body, &velocity);
        switch (enemy->direction)
        {
            case LEFT:
                velocity.x = -8.f;
                PE_Body_setVelocity(body, &velocity);
                enemy->m_state = NUT_SPINNING;
                break;
            case RIGHT:
                velocity.x = -8.f;
                PE_Body_setVelocity(body, &velocity);
                enemy->m_state = NUT_SPINNING;
                break;
            default:
                break;
                
        }
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
            Block_hit(GameObject_getBlock(otherObject));
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


void Nut_render(Enemy *enemy)
{

    Camera *camera = Scene_getCamera(enemy->m_object->m_scene);
    float viewX, viewY;
    PE_Vec2 position;
    PE_Body* body = GameObject_getBody(enemy->m_object);
    position.x = body->m_position.x;
    position.y = body->m_position.y + 1;
    Camera_worldToView(camera, &position, &viewX, &viewY);
    RE_Animator_renderF(enemy->m_animator, (int)viewX, (int)viewY);

}

void Nut_damage(Enemy *enemy)
{
    // TODO
    // Faire disparaître la noisette en cas de dommage.
    // Cette fonction peut être appelée par le joueur quand il entre en collision
    // avec un noisette et qu'il est situé au-dessus.
    Scene_removeObject(GameObject_getScene(Enemy_getObject(enemy)), Enemy_getObject(enemy));
}
