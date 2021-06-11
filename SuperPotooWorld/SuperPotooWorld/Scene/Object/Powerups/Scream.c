#include "Skill.h"
#include "../Collectable/Collectable.h"
#include "Scream.h"
#include "../../Scene.h"
#include <stdio.h>
#include <stdlib.h>


void Scream_onTriggerEnter(PE_Trigger* trigger)
{
    PE_Body* thisBody = PE_Trigger_getBody(trigger);
    PE_Body* otherBody = PE_Trigger_getOtherBody(trigger);
    GameObject* thisObject = (GameObject*)PE_Body_getUserData(thisBody);
    GameObject* otherObject = (GameObject*)PE_Body_getUserData(otherBody);
    Enemy* enemy = NULL;
    Block* block = NULL;

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Scream_onTriggerEnter()\n");
        return;
    }

    enemy = GameObject_getEnemy(otherObject);
    if (enemy)
    {
        Enemy_damage(enemy);
    }

    if(enemy || otherObject->m_type == GAME_BLOCK)
    {
        if(!GameObject_hasOneFlag(thisObject, OBJECT_TO_REMOVE))
        {
            Scene* scene = GameObject_getScene(thisObject);
            Scene_removeObject(scene, thisObject);
        }
    }    
    
    
}

int Scream_onStart(Skill* skill)
{
    Scene* scene = GameObject_getScene(skill->m_object);
    PE_Vec2* position = &skill->m_startPos;
    PE_World* world = NULL;
    PE_Body* body = NULL;
    PE_BodyDef bodyDef;
    PE_Collider* collider = NULL;
    PE_ColliderDef colliderDef;

    // Ajout dans le moteur physique
    world = Scene_getWorld(scene);

    // Création du corps associé
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_DYNAMIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(skill->m_object, body);

    // Création du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, .1f, .1f, .9f, .9f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_VISIBLE | FILTER_DAMAGER ;
    colliderDef.filter.maskBits = FILTER_ENEMY | FILTER_CAMERA | FILTER_BLOCK;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    // Callback du collider
    PE_Collider_setOnTriggerEnter(collider, Scream_onTriggerEnter);
    
    PE_Body_setGravityScale(body, 0);

    Player* player = Scene_getPlayer(scene);
    skill->direction = player->facingDirection;
    
    GameTextures* textures = Scene_getTextures(scene);
    RE_TextureAnim* texAnim = RE_Animator_createTextureAnim(skill->animator, textures->scream, "Scream");
    if (!texAnim) goto ERROR_LABEL;
    RE_TextureAnim_setCycleTime(texAnim, 0.3f);

    if(player->facingDirection == FACING_LEFT)
    {
        RE_Transform transform = RE_Transform_getDefault();
        transform.flipFlags = RE_FLIP_HORIZONTAL;
        RE_Animator_setTransform(skill->animator, &transform);
    }

    RE_Animator_playTextureAnim(skill->animator, "Scream");

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Scream_onStart()\n");
    return EXIT_FAILURE;
}

int Scream_onRespawn(Skill* skill)
{
    int exitStatus = Scream_onStart(skill);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Scream_onRespawn()\n");
    return EXIT_FAILURE;
}

int Scream_fixedUpdate(GameObject* object)
{
    Skill* skill = GameObject_getSkill(object);
    PE_Vec2 velocity;
    PE_Body_getVelocity(object->m_body, &velocity);
    velocity.x = skill->direction * 10.0f;
    velocity.y = 0.0f;
    PE_Body_setVelocity(GameObject_getBody(object), &velocity);
    return EXIT_SUCCESS;
}

void Scream_render(Skill* skill)
{
    Scene* scene = GameObject_getScene(skill->m_object);
    GameTextures* textures = Scene_getTextures(scene);
    GameAnimators* animators = Scene_getAnimators(scene);
    PE_Vec2 position = GameObject_getPosition(skill->m_object);
    Camera* camera = Scene_getCamera(scene);

    float x, y;
    position.x = position.x;
    position.y = position.y + 1;
    Camera_worldToView(camera, &position, &x, &y);
    RE_Animator_renderF(skill->animator, x, y);

}