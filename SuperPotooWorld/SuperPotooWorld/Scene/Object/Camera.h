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

/// @brief Structure représentant la caméra par laquelle est vue la scène.
typedef struct Camera_s
{
    GameObject *m_object;

    /// @brief Rectangle représentant la vue de la caméra.
    /// Ces coordonnées sont dans le référentiel monde.
    PE_AABB m_worldView;

    PE_AABB m_worldAABB;
    PE_AABB m_range;

    /// @brief Largeur en pixels de la caméra.
    int m_width;

    /// @brief Hauteur en pixels de la caméra.
    int m_height;

    /// @brief Echelle sur l'axes des abscisses.
    float m_xScale;

    /// @brief Echelle sur l'axes des ordonnées.
    float m_yScale;

    PHT *m_visibleObjects;
    PVector *m_sortedObjects;

    float m_speed;
} Camera;

/// @brief Crée une caméra.
/// @param[in] scene la scène vue par la caméra.
/// @return La caméra créée ou NULL en cas d'erreur.
Camera *Camera_new(Scene *scene, PE_AABB *worldView, PE_AABB *worldAABB, float speed);

INLINE GameObject *Camera_getObject(Camera *camera)
{
    return camera->m_object;
}

/// @brief Renvoie la zone du monde vue par la caméra.
/// @param[in] camera la caméra.
/// @param[out] worldView le rectangle vu par la caméra exprimé dans le référentiel monde.
void Camera_getWorldView(Camera *camera, PE_AABB *worldView);

/// @brief Renvoie la largeur en pixels de la caméra.
/// @param[in] camera la caméra.
/// @return La largeur en pixels de la caméra.
INLINE int Camera_getWidth(Camera *camera)
{
    return camera->m_width;
}

/// @brief Renvoie la hauteur en pixels de la caméra.
/// @param[in] camera la caméra.
/// @return La hauteur en pixels de la caméra.
INLINE int Camera_getHeight(Camera *camera)
{
    return camera->m_height;
}

/// @brief Transforme des coordonnées exprimées dans le référentiel monde vers le référentiel caméra.
/// @param[in] camera la caméra.
/// @param[in] position la position d'un point dans le référentiel monde.
/// @param[out] x l'abscisse du point dans le référentiel caméra (en pixels).
/// @param[out] y l'ordonnée du point dans le référentiel caméra (en pixels).
void Camera_worldToView(Camera *camera, PE_Vec2 *position, float *x, float *y);

/// @brief Transforme des coordonnées exprimées dans le référentiel caméra vers le référentiel monde.
/// @param[in] camera camera la caméra.
/// @param[in] x l'abscisse d'un point dans le référentiel caméra (en pixels).
/// @param[in] y l'ordonnée d'un point dans le référentiel caméra (en pixels).
/// @param[out] position la position du point dans le référentiel monde.
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
