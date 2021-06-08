#include "Block.h"
#include "Block_OneWay.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int OneWay_onStart(Block *block)
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
    colliderDef.filter.categoryBits = FILTER_BLOCK;
    colliderDef.filter.maskBits = FILTER_PLAYER | FILTER_ENEMY;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;
    printf("OneWay_onStart : Initialisation d'un bloc OneWay\n");

    return EXIT_SUCCESS;

    ERROR_LABEL:
        printf("ERROR - OneWay_onStart()\n");
    return EXIT_FAILURE;
}

// Les blocs OneWay n'ont pas de fonction render() puisqu'ils sont déjà dessinés dans la Tilemap.