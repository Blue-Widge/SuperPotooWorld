#ifndef _SCENE_H_
#define _SCENE_H_

#include "../Settings.h"
#include "../DataStructure/PHT.h"
#include "../DataStructure/PVector.h"

#include "ObjectManager.h"
#include "GameInput.h"
#include "GameTextures.h"
#include "GameAnimators.h"
#include "GameUI.h"
#include "Gizmos.h"
#include "Parallax.h"
#include "Tilemap.h"

#include "Object/Powerups//Skill.h"

#include "Object/GameObject.h"
#include "Object/Camera.h"
#include "Object/Player.h"
#include "Object/Block/Block.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Collectable/Collectable.h"
#include "Object/Tool/Tool.h"

#include <stdio.h>

#include "../UITextures.h"

typedef struct Scene_s
{
    PE_World *m_world;
    RE_Renderer *m_renderer;
    RE_Timer *m_time;

    float m_timeStep;
    float m_accu;
    float m_alpha;

    Bool m_respawn;
    Bool m_showGizmos;
    Bool m_gameOver;
    Bool m_finish;

    GameInput *m_input;
    GameTextures *m_textures;
    GameAnimators *m_animators;
    Camera *m_camera;
    Tilemap *m_tilemap;
    Player *m_player;
    Parallax *m_parallax;
    Gizmos *m_gizmos;

    ObjectManager *m_manager;

    PE_Vec2 m_startPos;

    UITextures* m_UItextures;
} Scene;

Scene *Scene_new(RE_Renderer *renderer, FILE *levelFile, RE_Timer *time, float timeStep);
void Scene_free(Scene *scene);

INLINE void Scene_respawn(Scene *scene)
{
    scene->m_respawn = TRUE;
}
INLINE void Scene_gameOver(Scene *scene)
{
    scene->m_gameOver = TRUE;
}
INLINE void Scene_finish(Scene *scene)
{
    scene->m_finish = TRUE;
}

int Scene_update(Scene *scene);

void Scene_render(Scene *scene);

// Accesseurs

INLINE RE_Renderer *Scene_getRenderer(Scene *scene)
{
    return scene->m_renderer;
}

INLINE GameTextures *Scene_getTextures(Scene *scene)
{
    return scene->m_textures;
}

INLINE GameAnimators *Scene_getAnimators(Scene *scene)
{
    return scene->m_animators;
}

INLINE RE_Timer *Scene_getTime(Scene *scene)
{
    return scene->m_time;
}

INLINE Camera *Scene_getCamera(Scene *scene)
{
    return scene->m_camera;
}

INLINE Player *Scene_getPlayer(Scene *scene)
{
    return scene->m_player;
}

INLINE PE_World *Scene_getWorld(Scene *scene)
{
    return scene->m_world;
}

INLINE GameInput *Scene_getInput(Scene *scene)
{
    return scene->m_input;
}

INLINE PE_Vec2 Scene_getStartPosition(Scene *scene)
{
    return scene->m_startPos;
}

INLINE float Scene_getAlpha(Scene *scene)
{
    return scene->m_alpha;
}


// Création d'objets

Collectable *Scene_createCollectable(Scene *scene, int type, PE_Vec2 *position);

Enemy *Scene_createEnemy(Scene *scene, int type, PE_Vec2 *position, int gravityDirection);

Block *Scene_createBlock(Scene *scene, int type, PE_Vec2 *position, PE_AABB *aabb);

Tool *Scene_createTool(Scene *scene, int type, PE_Vec2 *position, PE_AABB *aabb);

Skill* Scene_createSkill(Scene* scene, int type, PE_Vec2* position);

// Gestion des objets

INLINE int Scene_startOject(Scene *scene, GameObject *object)
{
    return ObjectManager_startObject(scene->m_manager, object);
}

INLINE int Scene_removeObject(Scene *scene, GameObject *object)
{
    return ObjectManager_removeObject(scene->m_manager, object);
}

INLINE int Scene_disableObject(Scene *scene, GameObject *object)
{
    return ObjectManager_disableObject(scene->m_manager, object);
}

INLINE int Scene_setToUpdate(Scene *scene, GameObject *object)
{
    return ObjectManager_setToUpdate(scene->m_manager, object);
}

INLINE int Scene_setToFixedUpdate(Scene *scene, GameObject *object)
{
    return ObjectManager_setToFixedUpdate(scene->m_manager, object);
}

INLINE int Scene_setToRespawn(Scene *scene, GameObject *object)
{
    return ObjectManager_setToRespawn(scene->m_manager, object);
}


#endif
