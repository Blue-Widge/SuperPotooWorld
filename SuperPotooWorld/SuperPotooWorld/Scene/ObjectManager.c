#include "ObjectManager.h"
#include "Scene.h"

#include <stdio.h>
#include <stdlib.h>

ObjectManager *ObjectManager_new(Scene *scene)
{
    ObjectManager *objManager = NULL;
    int objectCapacity = 1 << 11;
    int updateCapacity = 1 << 5;

    objManager = (ObjectManager *)calloc(1, sizeof(ObjectManager));
    if (!objManager) goto ERROR_LABEL;

    objManager->m_scene = scene;

    // Allocation des tables de hachage
    objManager->m_objects = PHT_new(objectCapacity);
    if (!objManager->m_objects) goto ERROR_LABEL;

    objManager->m_toRespawn = PHT_new(objectCapacity);
    if (!objManager->m_toRespawn) goto ERROR_LABEL;

    objManager->m_toUpdate = PHT_new(objectCapacity);
    if (!objManager->m_toUpdate) goto ERROR_LABEL;

    objManager->m_toFixedUpdate = PHT_new(objectCapacity);
    if (!objManager->m_toFixedUpdate) goto ERROR_LABEL;

    // Allocation des vecteurs
    objManager->m_toStart = PVector_new(objectCapacity);
    if (!objManager->m_toStart) goto ERROR_LABEL;

    objManager->m_toRemove = PVector_new(updateCapacity);
    if (!objManager->m_toRemove) goto ERROR_LABEL;

    objManager->m_toDisable = PVector_new(updateCapacity);
    if (!objManager->m_toDisable) goto ERROR_LABEL;

    objManager->m_buffer = PVector_new(objectCapacity);
    if (!objManager->m_buffer) goto ERROR_LABEL;

    return objManager;

ERROR_LABEL:
    printf("ERROR - ObjectManager_new\n");
    ObjectManager_free(objManager);
    return NULL;
}

void ObjectManager_free(ObjectManager *objManager)
{
    if (!objManager)
        return;

    if (objManager->m_objects)
    {
        GameObject **objects = (GameObject **)PHT_toArray(objManager->m_objects);
        int nbObjects = PHT_getSize(objManager->m_objects);

        for (int i = 0; i < nbObjects; i++)
        {
            GameObject_free(objects[i]);
        }
        PHT_free(objManager->m_objects);
    }
    PHT_free(objManager->m_toRespawn);
    PHT_free(objManager->m_toUpdate);
    PHT_free(objManager->m_toFixedUpdate);

    PVector_free(objManager->m_toStart);
    PVector_free(objManager->m_toDisable);
    PVector_free(objManager->m_toRemove);
    PVector_free(objManager->m_buffer);

    free(objManager);
}

int ObjectManager_manageObjects(ObjectManager *objManager)
{
    Camera *camera = Scene_getCamera(objManager->m_scene);
    int res = EXIT_SUCCESS;
    int nbObjects = 0;

    // Ajout des objets dans le moteur physique
    nbObjects = PVector_getSize(objManager->m_toStart);
    if (nbObjects > 0)
    {
        void **objects = PVector_toArray(objManager->m_toStart);

        for (int i = 0; i < nbObjects; i++)
        {
            GameObject *curObject = (GameObject *)objects[i];
            GameObject_removeFlags(curObject, OBJECT_TO_START);

            int exitStatus = GameObject_onStart(curObject);
            if (exitStatus != EXIT_SUCCESS)
            {
                printf("ERROR - ObjectManager_manageObjects()\n");
                printf("ERROR - Impossible to start the object %p\n", curObject);
                res = EXIT_FAILURE;
                continue;
            }
        }
        PVector_clear(objManager->m_toStart);
    }

    // Suppression des objets
    nbObjects = PVector_getSize(objManager->m_toRemove);
    if (nbObjects > 0)
    {
        void **objects = PVector_toArray(objManager->m_toRemove);

        for (int i = 0; i < nbObjects; i++)
        {
            GameObject *curObject = (GameObject *)objects[i];
            int exitStatus = PHT_remove(objManager->m_objects, curObject);
            if (exitStatus != EXIT_SUCCESS)
            {
                printf("ERROR - ObjectManager_manageObjects()\n");
                printf("ERROR - Impossible to remove the object %p\n", curObject);
                res = EXIT_FAILURE;
                continue;
            }

            exitStatus = PHT_remove(objManager->m_toRespawn, curObject);
            exitStatus = PHT_remove(objManager->m_toUpdate, curObject);
            exitStatus = PHT_remove(objManager->m_toFixedUpdate, curObject);
            exitStatus = Camera_removeVisibleObject(camera, curObject);

            // Destruction de l'objet
            GameObject_free(curObject);
        }
        PVector_clear(objManager->m_toRemove);
    }

    // Désactivation des objets
    nbObjects = PVector_getSize(objManager->m_toDisable);
    if (nbObjects > 0)
    {
        void **objects = PVector_toArray(objManager->m_toDisable);

        for (int i = 0; i < nbObjects; i++)
        {
            GameObject *curObject = (GameObject *)objects[i];
            GameObject_removeFlags(curObject, OBJECT_TO_DISABLE);

            int exitStatus = PHT_add(objManager->m_toRespawn, curObject);
            if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

            exitStatus = PHT_remove(objManager->m_toUpdate, curObject);
            exitStatus = PHT_remove(objManager->m_toFixedUpdate, curObject);
            exitStatus = Camera_removeVisibleObject(camera, curObject);

            // Destruction du corps associé dans le moteur physique
            GameObject_removeBody(curObject);
        }
        PVector_clear(objManager->m_toDisable);
    }

    return res;

ERROR_LABEL:
    printf("ERROR - ObjectManager_manageObjects()\n");
    return EXIT_FAILURE;
}

int ObjectManager_addObject(ObjectManager *objManager, GameObject *object)
{
    int exitStatus = PHT_add(objManager->m_objects, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - ObjectManager_addObject()\n");
    return EXIT_FAILURE;
}

void ObjectManager_printError(GameObject *object)
{
    if (GameObject_hasOneFlag(object, OBJECT_TO_START))
    {
        printf("ERROR - The game object is waiting to start\n");
    }
    if (GameObject_hasOneFlag(object, OBJECT_TO_DISABLE))
    {
        printf("ERROR - The game object is waiting to be disabled\n");
    }
    if (GameObject_hasOneFlag(object, OBJECT_TO_REMOVE))
    {
        printf("ERROR - The game object is waiting to be removed\n");
    }
}

int ObjectManager_startObject(ObjectManager *objManager, GameObject *object)
{
    if (GameObject_hasOneFlag(object, OBJECT_TO_START | OBJECT_TO_REMOVE | OBJECT_TO_DISABLE))
        goto ERROR_LABEL;

    PVector_pushBack(objManager->m_toStart, object);
    GameObject_addFlags(object, OBJECT_TO_START);
    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - ObjectManager_startObject()\n");
    ObjectManager_printError(object);
    return EXIT_FAILURE;
}

int ObjectManager_disableObject(ObjectManager *objManager, GameObject *object)
{
    if (GameObject_hasOneFlag(object, OBJECT_TO_START | OBJECT_TO_REMOVE | OBJECT_TO_DISABLE))
        goto ERROR_LABEL;

    PVector_pushBack(objManager->m_toDisable, object);
    GameObject_addFlags(object, OBJECT_TO_DISABLE);
    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - ObjectManager_disableObject()\n");
    ObjectManager_printError(object);
    return EXIT_FAILURE;
}

int ObjectManager_removeObject(ObjectManager *objManager, GameObject *object)
{
    if (GameObject_hasOneFlag(object, OBJECT_TO_START | OBJECT_TO_REMOVE | OBJECT_TO_DISABLE))
        goto ERROR_LABEL;

    PVector_pushBack(objManager->m_toRemove, object);
    GameObject_addFlags(object, OBJECT_TO_REMOVE);
    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - ObjectManager_removeObject()\n");
    ObjectManager_printError(object);
    return EXIT_FAILURE;
}

int ObjectManager_setToRespawn(ObjectManager *objManager, GameObject *object)
{
    return PHT_add(objManager->m_toRespawn, object);
}

int ObjectManager_setToUpdate(ObjectManager *objManager, GameObject *object)
{
    return PHT_add(objManager->m_toUpdate, object);
}

int ObjectManager_setToFixedUpdate(ObjectManager *objManager, GameObject *object)
{
    return PHT_add(objManager->m_toFixedUpdate, object);
}

int ObjectManager_update(ObjectManager *objManager)
{
    int nbObjects = PHT_getSize(objManager->m_toUpdate);
    if (nbObjects > 0)
    {
        void **objects = PHT_toArray(objManager->m_toUpdate);
        for (int i = 0; i < nbObjects; i++)
        {
            GameObject *curObject = (GameObject *)objects[i];
            int exitStatus = GameObject_update(curObject);
            if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;
        }
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - ObjectManager_update()\n");
    return EXIT_FAILURE;
}

int ObjectManager_fixedUpdate(ObjectManager *objManager)
{
    int nbObjects = PHT_getSize(objManager->m_toFixedUpdate);
    if (nbObjects > 0)
    {
        void **objects = PHT_toArray(objManager->m_toFixedUpdate);
        for (int i = 0; i < nbObjects; i++)
        {
            GameObject *curObject = (GameObject *)objects[i];
            int exitStatus = GameObject_fixedUpdate(curObject);
            if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;
        }
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - ObjectManager_fixedUpdate()\n");
    return EXIT_FAILURE;
}


int ObjectManager_respawn(ObjectManager *objManager)
{    
    printf("Start Respawn\n");
    int nbObjects = PHT_getSize(objManager->m_toRespawn);
    if (nbObjects > 0)
    {
        void **objects = PHT_toArray(objManager->m_toRespawn);

        printf("Respawn %d objects\n", nbObjects);

        PVector_copy(objManager->m_buffer, objects, nbObjects);
        for (int i = 0; i < nbObjects; i++)
        {
            GameObject_removeFlags((GameObject *)objects[i], OBJECT_ALL_FLAGS);
        }
        PHT_clear(objManager->m_toRespawn);

        objects = PVector_toArray(objManager->m_buffer);

        for (int i = 0; i < nbObjects; i++)
        {
            GameObject *curObject = (GameObject *)objects[i];
            int exitStatus = GameObject_onRespawn(curObject);
            if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;
        }
        PVector_clear(objManager->m_buffer);
    }

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - ObjectManager_respawn()\n");
    return EXIT_FAILURE;
}
