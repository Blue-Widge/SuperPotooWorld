#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

#include "../Settings.h"
#include "../DataStructure/PHT.h"
#include "../DataStructure/PVector.h"
#include "Object/GameObject.h"

typedef struct Scene_s Scene;

typedef struct ObjectManager_s
{
    Scene *m_scene;

    PHT *m_objects;
    PHT *m_toRespawn;
    PHT *m_toUpdate;
    PHT *m_toFixedUpdate;

    PVector *m_toRemove;
    PVector *m_toDisable;
    PVector *m_toStart;
    PVector *m_buffer;
} ObjectManager;

ObjectManager *ObjectManager_new(Scene *scene);
void ObjectManager_free(ObjectManager *objManager);
int ObjectManager_manageObjects(ObjectManager *objManager);

int ObjectManager_addObject(ObjectManager *objManager, GameObject *object);
int ObjectManager_startObject(ObjectManager *objManager, GameObject *object);
int ObjectManager_disableObject(ObjectManager *objManager, GameObject *object);
int ObjectManager_removeObject(ObjectManager *objManager, GameObject *object);

int ObjectManager_setToUpdate(ObjectManager *objManager, GameObject *object);
int ObjectManager_setToFixedUpdate(ObjectManager *objManager, GameObject *object);
int ObjectManager_setToRespawn(ObjectManager *objManager, GameObject *object);

int ObjectManager_update(ObjectManager *objManager);
int ObjectManager_fixedUpdate(ObjectManager *objManager);
int ObjectManager_respawn(ObjectManager *objManager);

#endif
