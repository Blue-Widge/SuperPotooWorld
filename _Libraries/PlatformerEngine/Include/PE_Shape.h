/// @file PE_Shape.h
/// @defgroup PE_Shape Fonctions relatives à la forme d'un collider

#ifndef _PE_SHAPE_H_
#define _PE_SHAPE_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"
#include "PE_Math.h"

/// @ingroup PE_Shape
/// @brief Typedef de l'énumération PE_ShapeType_e.
typedef enum PE_ShapeType_e PE_ShapeType;

/// @ingroup PE_Shape
/// @brief Enumération donnant les formes possibles pour un collider.
enum PE_ShapeType_e
{
    PE_SHAPE_BOX = 0,
    PE_SHAPE_TRIANGLE = 1,
};

typedef struct PE_BoxShape_s
{
    PE_Vec2 m_lower;
    PE_Vec2 m_upper;
} PE_BoxShape;

void PE_BoxShape_getAABB(PE_BoxShape *boxShape, PE_Vec2 *transform, PE_AABB *aabb);
extern PE_API void PE_BoxShape_set(PE_BoxShape *boxShape, float xLower, float yLower, float xUpper, float yUpper);

typedef enum PE_TriangleType_s
{
    PE_RIGHT_TRIANGLE_SW,
    PE_RIGHT_TRIANGLE_SE,
    PE_RIGHT_TRIANGLE_NW,
    PE_RIGHT_TRIANGLE_NE
} PE_TriangleType;

typedef struct PE_TriangleShape_s
{
    PE_Vec2 m_lower;
    PE_Vec2 m_upper;
    PE_Vec2 m_normal;
    int m_type;
} PE_TriangleShape;

void PE_TriangleShape_getAABB(PE_TriangleShape *triangleShape, PE_Vec2 *transform, PE_AABB *aabb);
extern PE_API void PE_TriangleShape_set(
    PE_TriangleShape *triangleShape, int type,
    float xLower, float yLower, float xUpper, float yUpper);

/// @ingroup PE_Shape
/// @brief Typedef de la structure PE_Shape_s.
typedef struct PE_Shape_s PE_Shape;

/// @ingroup PE_Shape
/// @brief Structure représentant la forme d'un collider
struct PE_Shape_s
{
    int m_type;
    float m_skin;
    union {
        PE_BoxShape box;
        PE_TriangleShape triangle;
    } m_data;
};

extern PE_API void PE_Shape_getAABB(PE_Shape *shape, PE_Vec2 *transform, PE_AABB *aabb);

/// @brief Renvoie le type d'une forme.
/// Les valeurs possible sont données dans l'énumération PE_ShapeType_e
/// @param[in] shape la forme.
/// @return Le type de la forme.
PE_API PE_INLINE int PE_Shape_getType(PE_Shape *shape)
{
    return shape->m_type;
}

/// @ingroup PE_Shape
/// @brief Définit une forme comme étant une boîte alignée sur les axes.
/// @param[out] shape la forme à initialiser.
/// @param[in] xLower abcisse du coin inférieur gauche de la boîte.
/// @param[in] yLower ordonnée du coin inférieur gauche de la boîte.
/// @param[in] xUpper abcisse du coin supérieur droit de la boîte.
/// @param[in] yUpper ordonnée du coin supérieur droit de la boîte.
PE_API PE_INLINE void PE_Shape_setAsBox(
    PE_Shape *shape, float xLower, float yLower, float xUpper, float yUpper)
{
    shape->m_type = PE_SHAPE_BOX;
    shape->m_skin = PE_SKIN_RADIUS;
    PE_BoxShape_set(&(shape->m_data.box), xLower, yLower, xUpper, yUpper);
}

/// @ingroup PE_Shape
/// @brief Définit une forme comme étant un triangle rectangle aligné sur les axes.
/// @param[out] shape la forme à initialiser.
/// @param[in] type le type du triangle. Les valeurs possibles sont données dans l'énumération PE_TriangleType
/// @param[in] xLower abcisse du coin inférieur gauche de la boîte.
/// @param[in] yLower ordonnée du coin inférieur gauche de la boîte.
/// @param[in] xUpper abcisse du coin supérieur droit de la boîte.
/// @param[in] yUpper ordonnée du coin supérieur droit de la boîte.
PE_API PE_INLINE void PE_Shape_setAsTriangle(
    PE_Shape *shape, int type,
    float xLower, float yLower, float xUpper, float yUpper)
{
    shape->m_type = PE_SHAPE_TRIANGLE;
    shape->m_skin = PE_SKIN_RADIUS;
    PE_TriangleShape_set(&(shape->m_data.triangle), type, xLower, yLower, xUpper, yUpper);
}

// Methodes internes

PE_INLINE float PE_Shape_getSkin(PE_Shape *shape)
{
    return shape->m_skin;
}

#endif
