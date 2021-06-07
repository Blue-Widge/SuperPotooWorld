/// @file PE_Math.h
/// @defgroup PE_Math Fonctions utilitaires

#ifndef _PE_MATH_H_
#define _PE_MATH_H_

#include "PE_Settings.h"
#include <string.h>

/// @ingroup PE_Math
/// @brief Structure représentant un vecteur 2D dont les composantes sont de type float.
typedef struct PE_Vec2_s {

    /// @brief Abscisse du vecteur.
    float x;

    /// @brief Ordonnée du vecteur.
    float y;
} PE_Vec2;

/// @ingroup PE_Math
/// @brief Initialise les coordonnées d'un vecteur.
/// @param[in,out] v le vecteur à initialiser.
/// @param[in] x l'abscisse du vecteur. 
/// @param[in] y l'ordonnée du vecteur.
extern PE_API void PE_Vec2_set(PE_Vec2 *v, float x, float y);

PE_API PE_INLINE void PE_Vec2_copy(PE_Vec2 *destination, PE_Vec2 *source)
{
    PE_Memcpy(destination, sizeof(PE_Vec2), source, sizeof(PE_Vec2));
}

extern PE_API void PE_Vec2_addTo(PE_Vec2 *v, PE_Vec2 *add);
extern PE_API void PE_Vec2_multiplyBy(PE_Vec2 *v, float lambda);
extern PE_API float PE_Vec2_dot(PE_Vec2 *v1, PE_Vec2 *v2);

/// @ingroup PE_Math
/// @brief Structure représentant une boîte englobante (rectangle) dont les côtés sont alignés sur les axes.
/// AABB signifie "Aligned Axes Bounding Box".
typedef struct PE_AABB_s {

    /// @brief Coordonnées du coin inférieur gauche de la boîte.
    PE_Vec2 lower;

    /// @brief Coordonnées du coin supérieur droit de la boîte.
    PE_Vec2 upper;
} PE_AABB;

/// @ingroup PE_Math
/// @brief Initialise une AABB.
/// @param[in,out] aabb la boîte a initialiser.
/// @param[in] xLower abcisse du coin inférieur gauche de la boîte.
/// @param[in] yLower ordonnée du coin inférieur gauche de la boîte.
/// @param[in] xUpper abcisse du coin supérieur droit de la boîte.
/// @param[in] yUpper ordonnée du coin supérieur droit de la boîte.
extern PE_API void PE_AABB_set(PE_AABB *aabb, float xLower, float yLower, float xUpper, float yUpper);

extern PE_API Bool PE_AABB_overlap(PE_AABB *aabb1, PE_AABB *aabb2);

extern PE_API void PE_AABB_combine(PE_AABB *aabb1, PE_AABB *aabb2, PE_AABB *res);

extern PE_API void PE_AABB_translate(PE_AABB *aabb, PE_Vec2 *transform);

extern PE_API float PE_AABB_distance(PE_AABB *aabb1, PE_AABB *aabb2);

/// @brief Structure décrivant le mouvement d'un point pendant la mise à jour du monde.
typedef struct PE_Sweep_s
{
    /// @brief Position au ratio alpha0
    PE_Vec2 position0;

    /// @brief Position finale.
    PE_Vec2 position1;

    /// @brief Ratio entre la position avant et après la mise à jour du monde.
    float alpha0;
} PE_Sweep;

PE_INLINE void PE_Sweep_copy(PE_Sweep *destination, PE_Sweep *source)
{
    PE_Memcpy(destination, sizeof(PE_Sweep), source, sizeof(PE_Sweep));
}

void PE_Sweep_getTransform(PE_Sweep *sweep, float alpha, PE_Vec2 *transform);

#endif
