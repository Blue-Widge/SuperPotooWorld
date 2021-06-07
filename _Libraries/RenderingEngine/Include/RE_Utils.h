
/// @file RE_Utils.h
/// @defgroup RE_Utils Fonctions utilitaires

#ifndef _RE_UTILS_H_
#define _RE_UTILS_H_

#include "RE_Lib.h"
#include <string.h>

/// @ingroup RE_Utils
/// @brief Structure représenant un rectangle 2D alligné sur les axes X et Y.
typedef struct Rect_s
{
    /// @brief Abscisse du sommet en bas à gauche du rectangle.
    float x;

    /// @brief Ordonnée du sommet en bas à gauche du rectangle.
    float y;

    /// @brief Largeur du rectangle.
    float w;

    /// @brief Hauteur du rectangle.
    float h;
} Rect;

/// @ingroup RE_Utils
/// @brief Construit un rectangle.
/// @param[in] x abscisse du sommet en bas à gauche du rectangle.
/// @param[in] y ordonnée du sommet en bas à gauche du rectangle.
/// @param[in] w largeur du rectangle.
/// @param[in] h hauteur du rectangle.
/// @return Le rectangle spécifié.
extern RE_API Rect Rect_set(float x, float y, float w, float h);

/// @ingroup RE_Utils
/// @brief Structure représentant un vecteur 2D dont les composantes sont de type float.
typedef struct Vec2_s
{
    /// @brief Abscisse du vecteur.
    float x;

    /// @brief Ordonnée du vecteur.
    float y;
} Vec2;

/// @ingroup RE_Utils
/// @brief Structure représentant un vecteur 2D dont les composantes sont de type int.
typedef struct Vec2i_s
{
    /// @brief Abscisse du vecteur.
    int x;

    /// @brief Ordonnée du vecteur.
    int y;
} Vec2i;

/// @ingroup RE_Utils
/// @brief Construit un vecteur à partir de ses composantes.
/// @param[in] x la composante x.
/// @param[in] y la composante y.
/// @return Le vecteur ayant les composantes données.
extern RE_API Vec2 Vec2_set(float x, float y);

/// @ingroup RE_Utils
/// @brief Indique si un rectangle contient un point.
/// @param[in] rect le rectangle.
/// @param[in] point le point.
/// @return TRUE si le rectangle contient le point, FALSE sinon.
RE_API RE_INLINE int Rect_contains(Rect *rect, Vec2 *point)
{
    return (point->x >= rect->x) && (point->x <= rect->x + rect->w)
        && (point->y >= rect->y) && (point->y <= rect->y + rect->h);
}

#endif
