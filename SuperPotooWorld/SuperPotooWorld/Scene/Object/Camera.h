#ifndef _CAMERA_H_
#define _CAMERA_H_

/// @file Camera.h
/// @defgroup Camera
/// @{

#include "../../Settings.h"
#include "../../DataStructure/PHT.h"
#include "../../DataStructure/PVector.h"
#include "GameObject.h"

typedef struct Scene_s Scene;

/// @brief Structure repr�sentant la cam�ra par laquelle est vue la sc�ne.
typedef struct Camera_s
{
    GameObject *m_object;

    /// @brief Rectangle repr�sentant la vue de la cam�ra.
    /// Ces coordonn�es sont dans le r�f�rentiel monde.
    PE_AABB m_worldView;

    PE_AABB m_worldAABB;
    PE_AABB m_range;

    /// @brief Largeur en pixels de la cam�ra.
    int m_width;

    /// @brief Hauteur en pixels de la cam�ra.
    int m_height;

    /// @brief Echelle sur l'axes des abscisses.
    float m_xScale;

    /// @brief Echelle sur l'axes des ordonn�es.
    float m_yScale;

    PHT *m_visibleObjects;
    PVector *m_sortedObjects;

    float m_speed;
} Camera;

/// @brief Cr�e une cam�ra.
/// @param[in] scene la sc�ne vue par la cam�ra.
/// @return La cam�ra cr��e ou NULL en cas d'erreur.
Camera *Camera_new(Scene *scene, PE_AABB *worldView, PE_AABB *worldAABB, float speed);

INLINE GameObject *Camera_getObject(Camera *camera)
{
    return camera->m_object;
}

/// @brief Renvoie la zone du monde vue par la cam�ra.
/// @param[in] camera la cam�ra.
/// @param[out] worldView le rectangle vu par la cam�ra exprim� dans le r�f�rentiel monde.
void Camera_getWorldView(Camera *camera, PE_AABB *worldView);

/// @brief Renvoie la largeur en pixels de la cam�ra.
/// @param[in] camera la cam�ra.
/// @return La largeur en pixels de la cam�ra.
INLINE int Camera_getWidth(Camera *camera)
{
    return camera->m_width;
}

/// @brief Renvoie la hauteur en pixels de la cam�ra.
/// @param[in] camera la cam�ra.
/// @return La hauteur en pixels de la cam�ra.
INLINE int Camera_getHeight(Camera *camera)
{
    return camera->m_height;
}

/// @brief Transforme des coordonn�es exprim�es dans le r�f�rentiel monde vers le r�f�rentiel cam�ra.
/// @param[in] camera la cam�ra.
/// @param[in] position la position d'un point dans le r�f�rentiel monde.
/// @param[out] x l'abscisse du point dans le r�f�rentiel cam�ra (en pixels).
/// @param[out] y l'ordonn�e du point dans le r�f�rentiel cam�ra (en pixels).
void Camera_worldToView(Camera *camera, PE_Vec2 *position, float *x, float *y);

/// @brief Transforme des coordonn�es exprim�es dans le r�f�rentiel cam�ra vers le r�f�rentiel monde.
/// @param[in] camera camera la cam�ra.
/// @param[in] x l'abscisse d'un point dans le r�f�rentiel cam�ra (en pixels).
/// @param[in] y l'ordonn�e d'un point dans le r�f�rentiel cam�ra (en pixels).
/// @param[out] position la position du point dans le r�f�rentiel monde.
void Camera_viewToWorld(Camera *camera, float x, float y, PE_Vec2 *position);

INLINE int Camera_removeVisibleObject(Camera *camera, GameObject *object)
{
    return PHT_remove(camera->m_visibleObjects, object);
}

GameObject **Camera_getVisibleObjects(Camera *camera);

INLINE int Camera_getNbVisibleObjects(Camera *camera)
{
    return PHT_getSize(camera->m_visibleObjects);
}

/// @}

#endif
