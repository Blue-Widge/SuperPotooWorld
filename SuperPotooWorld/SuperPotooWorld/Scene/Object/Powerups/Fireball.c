#include "Skill.h"
#include "../Collectable/Collectable.h"
#include "FireBall.h"
#include "../../Scene.h"
#include <stdio.h>
#include <stdlib.h>


void FireBall_onTriggerEnter(PE_Trigger* trigger)
{
    PE_Body* thisBody = PE_Trigger_getBody(trigger);
    PE_Body* otherBody = PE_Trigger_getOtherBody(trigger);
    GameObject* thisObject = (GameObject*)PE_Body_getUserData(thisBody);
    GameObject* otherObject = (GameObject*)PE_Body_getUserData(otherBody);
    Enemy* enemy = NULL;

    if (!thisObject || !otherObject)
    {
        printf("ERROR - FireBall_onTriggerEnter()\n");
        return;
    }

    enemy = GameObject_getEnemy(otherObject);

    if (enemy)
    {
        Scene* scene = GameObject_getScene(thisObject);
        Scene_disableObject(scene, thisObject);
        Enemy_damage(enemy);
    }
}

int FireBall_onStart(Skill* skill)
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
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(skill->m_object, body);

    // Création du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape, 0.0, 0.0, 1.0f, 1.0f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_ENEMY | FILTER_BLOCK | FILTER_CAMERA | FILTER_DAMAGER;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    // Callback du collider
    PE_Collider_setOnTriggerEnter(collider, FireBall_onTriggerEnter);

    RE_Animator* animator = Scene_getAnimators(scene)->fireball;
    RE_Animator_playTextureAnim(animator, "Fireball");

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - FireBall_onStart()\n");
    return EXIT_FAILURE;
}

int FireBall_onRespawn(Skill* skill)
{
    int exitStatus = FireBall_onStart(skill);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - FireBall_onRespawn()\n");
    return EXIT_FAILURE;
}

void FireBall_render(Skill* skill)
{
    Scene* scene = GameObject_getScene(skill->m_object);
    GameTextures* textures = Scene_getTextures(scene);
    GameAnimators* animators = Scene_getAnimators(scene);
    PE_Vec2 position = GameObject_getPosition(skill->m_object);
    Camera* camera = Scene_getCamera(scene);

    float x, y;
    position.x = skill->m_startPos.x;
    position.y = skill->m_startPos.y + 1;
    Camera_worldToView(camera, &position, &x, &y);
    RE_Animator_renderF(animators->fireball, x, y);

}