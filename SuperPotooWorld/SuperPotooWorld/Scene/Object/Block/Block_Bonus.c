#include "Block.h"
#include "Block_Bonus.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int Bonus_onStart(Block *block)
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

    // Création du corps associé
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = *position;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(block->m_object, body);

    // Création du collider
    PE_ColliderDef_setDefault(&colliderDef);
    PE_Shape_setAsBox(&colliderDef.shape,
                      aabb->lower.x, aabb->lower.y, aabb->upper.x, aabb->upper.y);
    colliderDef.filter.categoryBits = FILTER_BLOCK | FILTER_VISIBLE;
    colliderDef.filter.maskBits = FILTER_BLOCK | FILTER_PLAYER | FILTER_ENEMY | FILTER_CAMERA | FILTER_COLLECTABLE;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    RE_Animator* animator = Scene_getAnimators(scene)->bonus;
    RE_Animator_playTextureAnim(animator, "Bonus");

    return EXIT_SUCCESS;

    ERROR_LABEL:
        printf("ERROR - Bonus_onStart()\n");
    return EXIT_FAILURE;
}

int Bonus_onRespawn(Block *block)
{
    return EXIT_SUCCESS;
}

void Bonus_render(Block *block)
{
    Camera *camera = Scene_getCamera(block->m_object->m_scene);
    GameTextures *textures = Scene_getTextures(block->m_object->m_scene);
    float viewX, viewY;
    PE_Vec2 position;
    position.x = block->m_startPos.x;
    position.y = block->m_startPos.y + 1;
    Camera_worldToView(camera, &position, &viewX, &viewY);
    if(block->m_nbHits == 0)
    {
        GameAnimators *animators = Scene_getAnimators(block->m_object->m_scene);
        RE_Animator_renderF(animators->bonus, (int)viewX, (int)viewY);
    }
    else
    {
        RE_Texture_render(textures->bonus_empty, 0, (int)viewX, (int)viewY);
    }
}

void Bonus_hit(Block *block)
{

    if(!block->m_nbHits)
    {
        PE_Vec2 position = block->m_startPos;
        position.y += 1.f;
        Scene_createCollectable(GameObject_getScene(block->m_object), POWERUP_FIRE, &position);
    }

    ++block->m_nbHits;

    if (block->m_nbHits == 1)
    {
        PE_Vec2 position = block->m_startPos;
        position.y += 1.f;
        CollectableType collectable = rand()%(POWERUP_SCREAM - POWERUP_FIRE + 1) + POWERUP_FIRE;
        switch (collectable)
        {
            case POWERUP_FIRE:
                Scene_createCollectable(GameObject_getScene(block->m_object), POWERUP_FIRE, &position);
            break;
            case POWERUP_SCREAM:
                Scene_createCollectable(GameObject_getScene(block->m_object), POWERUP_SCREAM, &position);
            break;
            default:
                break;
        }
    }
}
