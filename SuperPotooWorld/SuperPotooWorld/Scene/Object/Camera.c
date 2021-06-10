#include "Camera.h"
#include "../Scene.h"
#include "../../DataStructure/PHT.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Méthodes du GameObject
int Camera_free(GameObject *object);
int Camera_onStart(GameObject *object);
int Camera_update(GameObject *object);
int Camera_fixedUpdate(GameObject *object);

//-------------------------------------------------------------------------------------------------
//  Fonctions auxiliaires

/// @brief Met à jour les échelles interne à la caméra
/// @param camera la caméra.
void Camera_updateScales(Camera *camera)
{
    PE_AABB *worldView = &camera->m_worldView;
    float viewWidth, viewHeight;
    viewWidth  = worldView->upper.x - worldView->lower.x;
    viewHeight = worldView->upper.y - worldView->lower.y;

    camera->m_xScale = camera->m_width  / viewWidth;
    camera->m_yScale = camera->m_height / viewHeight;

    camera->m_range.lower.x = camera->m_worldAABB.lower.x;
    camera->m_range.lower.y = camera->m_worldAABB.lower.y;
    camera->m_range.upper.x = camera->m_worldAABB.upper.x - viewWidth;
    camera->m_range.upper.y = camera->m_worldAABB.upper.y - viewHeight;
}

void Camera_onTriggerEnter(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    PE_Body *otherBody = PE_Trigger_getOtherBody(trigger);
    GameObject *thisObject = (GameObject *)PE_Body_getUserData(thisBody);
    GameObject *otherObject = (GameObject *)PE_Body_getUserData(otherBody);

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Camera_onTriggerEnter()\n");
        return;
    }

    Camera *camera = GameObject_getCamera(thisObject);
    if (camera)
    {
        PHT_add(camera->m_visibleObjects, otherObject);
    }
}

void Camera_onTriggerExit(PE_Trigger *trigger)
{
    PE_Body *thisBody = PE_Trigger_getBody(trigger);
    PE_Body *otherBody = PE_Trigger_getOtherBody(trigger);
    GameObject *thisObject = (GameObject *)PE_Body_getUserData(thisBody);
    GameObject *otherObject = (GameObject *)PE_Body_getUserData(otherBody);

    if (!thisObject || !otherObject)
    {
        printf("ERROR - Camera_onTriggerExit()\n");
        return;
    }

    Camera *camera = GameObject_getCamera(thisObject);
    if (camera)
    {
        PHT_remove(camera->m_visibleObjects, otherObject);
    }
}

//-------------------------------------------------------------------------------------------------
//  Fonctions principales

Camera *Camera_new(Scene *scene, PE_AABB *worldView, PE_AABB *worldAABB, float speed)
{
    GameObject *object = NULL;
    Camera *camera = NULL;
    RE_Renderer *renderer = NULL;

    renderer = Scene_getRenderer(scene);

    object = GameObject_new(scene, GAME_CAMERA);
    if (!object) goto ERROR_LABEL;

    camera = (Camera *)calloc(1, sizeof(Camera));
    if (!camera) goto ERROR_LABEL;

    camera->m_object = object;
    camera->m_width = RE_Renderer_getWidth(renderer);
    camera->m_height = RE_Renderer_getHeight(renderer);
    camera->m_speed = speed;

    // Initialisation de la classe mère
    object->m_data = (void *)camera;
    object->cm_free = Camera_free;
    object->cm_onStart = Camera_onStart;
    object->cm_update = Camera_update;
    object->cm_fixedUpdate = Camera_fixedUpdate;

    PE_Memcpy(
        &camera->m_worldView, sizeof(PE_AABB),
        worldView, sizeof(PE_AABB));
    PE_Memcpy(
        &camera->m_worldAABB, sizeof(PE_AABB),
        worldAABB, sizeof(PE_AABB));
    Camera_updateScales(camera);

    // Allocations supplémentaires
    camera->m_visibleObjects = PHT_new(1 << 10);
    if (!camera->m_visibleObjects) goto ERROR_LABEL;

    camera->m_sortedObjects = PVector_new(1 << 10);
    if (!camera->m_sortedObjects) goto ERROR_LABEL;

    return camera;

ERROR_LABEL:
    printf("ERROR - Camera_new()\n");
    GameObject_free(object);
    return NULL;
}

int Camera_free(GameObject *object)
{
    Camera *camera = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    camera = GameObject_getCamera(object);
    if (!camera) goto ERROR_LABEL;

    PHT_free(camera->m_visibleObjects);
    PVector_free(camera->m_sortedObjects);

    free(camera);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Camera_free()\n");
    return EXIT_FAILURE;
}

int Camera_onStart(GameObject *object)
{
    Scene *scene = NULL;
    Camera *camera = NULL;
    PE_World *world = NULL;
    PE_Body *body = NULL;
    PE_BodyDef bodyDef;
    PE_Collider *collider = NULL;
    PE_ColliderDef colliderDef;
    float w, h;
    int exitStatus;

    if (!object || !object->m_data) goto ERROR_LABEL;

    camera = GameObject_getCamera(object);
    if (!camera) goto ERROR_LABEL;

    scene = GameObject_getScene(object);
    world = Scene_getWorld(scene);

    // Création du corps associé
    PE_BodyDef_setDefault(&bodyDef);
    bodyDef.type = PE_DYNAMIC_BODY;
    bodyDef.position = camera->m_worldView.lower;
    bodyDef.gravityScale = 0.f;
    body = PE_World_createBody(world, &bodyDef);
    if (!body) goto ERROR_LABEL;

    GameObject_setBody(camera->m_object, body);

    // Création du collider
    PE_ColliderDef_setDefault(&colliderDef);
    w = camera->m_worldView.upper.x - camera->m_worldView.lower.x;
    h = camera->m_worldView.upper.y - camera->m_worldView.lower.y;
    PE_Shape_setAsBox(&colliderDef.shape, -1.f, -1.f, w + 1.f, h + 1.f);
    colliderDef.isTrigger = TRUE;
    colliderDef.filter.categoryBits = FILTER_CAMERA;
    colliderDef.filter.maskBits = FILTER_VISIBLE;
    collider = PE_Body_createCollider(body, &colliderDef);
    if (!collider) goto ERROR_LABEL;

    // Callback du collider
    PE_Collider_setOnTriggerEnter(collider, Camera_onTriggerEnter);
    PE_Collider_setOnTriggerExit(collider, Camera_onTriggerExit);

    // Activation des méthodes de la classe mère
    exitStatus = Scene_setToUpdate(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    exitStatus = Scene_setToFixedUpdate(scene, object);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Camera_onStart()\n");
    return EXIT_FAILURE;
}

float clamp01(float value)
{
    if (value < 0.0f)
        return 0.0f;
    if (value > 1.0f)
        return 1.0f;
    return value;
}

float lerpf(float a, float b, float t)
{
    return a + (b - a) * t;
}

int Camera_update(GameObject *object)
{
    Scene *scene = NULL;
    Camera *camera = NULL;
    Player *player = NULL;
    PE_Vec2 playerPos, cameraPos, displacement;
    float x;

    if (!object || !object->m_data) goto ERROR_LABEL;

    camera = GameObject_getCamera(object);
    if (!camera) goto ERROR_LABEL;

    scene = GameObject_getScene(object);
    player = Scene_getPlayer(scene);
    playerPos = GameObject_getPosition(Player_getObject(player));
    cameraPos = camera->m_worldView.lower;

    float diffX = playerPos.x < 10.0f ? 0.0f : playerPos.x - 10.0f;
    float diffX2 = playerPos.x > scene->m_tilemap->m_width - 14.0f ? scene->m_tilemap->m_width - 24.3f : diffX;
    
    float diffY = playerPos.y < 5.0f ? 0.0f : playerPos.y - 5.0f;
    float diffY2 = playerPos.y + 8.0f > scene->m_tilemap->m_height ? scene->m_tilemap->m_height - 13.7f : diffY;
    
    float lerpX = lerpf(cameraPos.x,diffX2, 5.0f * RE_Timer_getDelta(Scene_getTime(scene)));
    float lerpY = lerpf(cameraPos.y, diffY2, 5.0f * RE_Timer_getDelta(Scene_getTime(scene)));
    
    displacement.x = lerpX - cameraPos.x;
    displacement.y = lerpY - cameraPos.y;
    PE_AABB_translate(&camera->m_worldView, &displacement);
    
    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Camera_update()\n");
    return EXIT_FAILURE;
}

int Camera_fixedUpdate(GameObject *object)
{
    Camera *camera = NULL;
    PE_Body *body = NULL;

    if (!object || !object->m_data) goto ERROR_LABEL;

    camera = GameObject_getCamera(object);
    if (!camera) goto ERROR_LABEL;

    body = GameObject_getBody(object);
    if (!body) goto ERROR_LABEL;

    PE_Body_setPosition(body, &(camera->m_worldView.lower));

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - Camera_fixedUpdate()\n");
    return EXIT_FAILURE;
}

void Camera_worldToView(Camera *camera, PE_Vec2 *position, float *x, float *y)
{
    *x = (position->x - camera->m_worldView.lower.x) * camera->m_xScale;
    *y = (position->y - camera->m_worldView.lower.y) * camera->m_yScale;
    *y = (float)camera->m_height - *y;
}

void Camera_viewToWorld(Camera *camera, float x, float y, PE_Vec2 *position)
{
    y = camera->m_height - y;
    float ratioX = (float)x / (float)camera->m_width;
    float ratioY = (float)y / (float)camera->m_height;
    float w = camera->m_worldView.upper.x - camera->m_worldView.lower.x;
    float h = camera->m_worldView.upper.y - camera->m_worldView.lower.y;
    position->x = camera->m_worldView.lower.x + ratioX * w;
    position->y = camera->m_worldView.lower.y + ratioY * h;
}


int ObjectComparator(const void *eltA, const void *eltB)
{
    GameObject *objectA = *(GameObject **)eltA;
    GameObject *objectB = *(GameObject **)eltB;
    return objectA->m_layer - objectB->m_layer;
}

GameObject **Camera_getVisibleObjects(Camera *camera)
{
    int size = PHT_getSize(camera->m_visibleObjects);
    PVector_copy(camera->m_sortedObjects, PHT_toArray(camera->m_visibleObjects), size);
    void **objects = PVector_toArray(camera->m_sortedObjects);
    qsort((void *)objects, size, sizeof(void *), ObjectComparator);
    return (GameObject **)objects;
}

void Camera_getWorldView(Camera *camera, PE_AABB *worldView)
{
    PE_Memcpy(worldView, sizeof(PE_AABB), &camera->m_worldView, sizeof(PE_AABB));
}