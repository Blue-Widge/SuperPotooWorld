
/// @file RE_Renderer.h
/// @defgroup RE_Renderer Fonctions relatives au renderer

#ifndef _RE_RENDERER_H_
#define _RE_RENDERER_H_

#include "RE_Lib.h"
#include "RE_Utils.h"

#include <stdint.h>
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

/// @ingroup RE_Renderer
/// @brief Structure représentant une couleur au format RGBA.
typedef struct RE_Color_s
{
    /// @brief L'intensité de la composante rouge entre 0 et 255.
    uint8_t r;

    /// @brief L'intensité de la composante verte entre 0 et 255.
    uint8_t g;

    /// @brief L'intensité de la composante bleue entre 0 et 255.
    uint8_t b;

    /// @brief La composante alpha (transparence) de la couleur.
    /// La couleur est parfaitement transparante quand elle vaut 0 et elle est opaque quand elle vaut 255.
    uint8_t a;
} RE_Color;

/// @ingroup RE_Renderer
/// @brief Construit une couleur à partir de ses composantes [R,V,B,A] chacune entre 0 et 255.
/// @param[in] r l'intensité du rouge.
/// @param[in] g l'intensité du vert.
/// @param[in] b l'intensité du bleu.
/// @param[in] a la composante alpha (transparence) de la couleur.
/// La couleur est parfaitement transparante quand elle vaut 0 et elle est opaque quand elle vaut 255.
/// @return La couleur spécifiée.
extern RE_API RE_Color RE_Color_set(int r, int g, int b, int a);

/// @ingroup RE_Renderer
/// @brief Structure représentant le moteur de rendu.
typedef struct RE_Renderer_s
{
    /// @protected
    /// @brief Le moteur de rendu SDL.
    SDL_Renderer *m_rendererSDL;

    /// @protected
    /// @brief La largeur en pixels de l'image rendue.
    int m_width;

    /// @protected
    /// @brief La hauteur en pixels de l'image rendue.
    int m_height;
} RE_Renderer;

/// @ingroup RE_Renderer
/// @brief Renvoie la largeur du moteur de rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @return La largeur du moteur de rendu.
extern RE_API int RE_Renderer_getWidth(RE_Renderer *renderer);

/// @ingroup RE_Renderer
/// @brief Renvoie la hauteur du moteur de rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @return La hauteur du moteur de rendu.
extern RE_API int RE_Renderer_getHeight(RE_Renderer *renderer);

/// @ingroup RE_Renderer
/// @brief Met à jour le moteur de rendu.
/// Cette fonction permet de d'afficher à l'écran la dernière image rendue.
/// @param[in,out] renderer le moteur de rendu.
extern RE_API void RE_Renderer_update(RE_Renderer *renderer);

/// @ingroup RE_Renderer
/// @brief Efface le rendu précédent. Le rendu devient entièrement noir.
/// @param[in,out] renderer le moteur de rendu.
extern RE_API void RE_Renderer_clear(RE_Renderer *renderer);

/// @ingroup RE_Renderer
/// @brief Définit la taille logique du moteur de rendu.
/// Si les dimensions données sont différentes de la résolution de la fenêtre,
/// le rendu est redimensionné.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] width la largeur logique du moteur de rendu.
/// @param[out] height la hauteur logique du moteur de rendu. 
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
extern RE_API int RE_Renderer_setLogicalSize(RE_Renderer *renderer, int width, int height);

/// @ingroup RE_Renderer
/// @brief Remplit le rendu avec une couleur donnée.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] color la couleur de remplissage.
extern RE_API void RE_Renderer_fill(RE_Renderer* renderer, RE_Color color);

/// @ingroup RE_Renderer
/// @brief Dessine un segment spécifié par deux points sur le rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] x1 abscisse du premier point (en pixels).
/// @param[in] y1 ordonnée du premier point (en pixels).
/// @param[in] x2 abscisse du second point (en pixels).
/// @param[in] y2 ordonnée du second point (en pixels).
/// @param[in] color la couleur de la ligne.
extern RE_API void RE_Renderer_drawLine(RE_Renderer *renderer, int x1, int y1, int x2, int y2, RE_Color color);

/// @ingroup RE_Renderer
/// @brief Dessine un point sur le rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] x abscisse du point (en pixels).
/// @param[in] y ordonnée du point (en pixels).
/// @param[in] color la couleur du point.
extern RE_API void RE_Renderer_drawPoint(RE_Renderer *renderer, int x, int y, RE_Color color);

/// @ingroup RE_Renderer
/// @brief Dessine un rectangle sur le rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] rect la spécification du rectangle à dessiner.
/// @param[in] color la couleur du point.
extern RE_API void RE_Renderer_drawRect(RE_Renderer *renderer, Rect *rect, RE_Color color);

/// @ingroup RE_Renderer
/// @brief Dessine un rectangle plein sur le rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] rect la spécification du rectangle à dessiner.
/// @param[in] color la couleur du point.
extern RE_API void RE_Renderer_fillRect(RE_Renderer *renderer, Rect *rect, RE_Color color);

#endif