
/// @file RE_Window.h
/// @defgroup RE_Window Fonctions relatives à la fenêtre principale

#ifndef _RE_WINDOW_H_
#define _RE_WINDOW_H_

#include "RE_Lib.h"
#include "RE_Renderer.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

/// @ingroup RE_Window
/// @brief Structure représentant une fenêtre SDL avec son moteur de rendu.
typedef struct RE_Window_s
{
    /// @protected
    /// @brief La fenêtre SDL.
    SDL_Window  *m_windowSDL;

    /// @protected
    /// @brief Le moteur de rendu associé à la fenêtre.
    RE_Renderer *m_renderer;
} RE_Window;

/// @ingroup RE_Window
/// @brief Listes des flags utilisés lors de la création d'une fenêtre.
typedef enum RE_WindowFlags
{
    RE_WINDOW_FULLSCREEN = SDL_WINDOW_FULLSCREEN,
} RE_WindowFlags;

/// @ingroup RE_Window
/// @brief Crée une nouvelle fenêtre.
/// @param[in] title le titre de la fenêtre.
/// @param[in] width la largeur (en pixels) de la fenêtre.
/// @param[in] height la hauteur (en pixels) de la fenêtre.
/// @param[in] flags les flags de la fenêtre.
///       Les valeurs possibles des flags sont données dans l'énumération RE_WindowFlags.
/// @return La fenêtre créée ou NULL en cas d'erreur.
extern RE_API RE_Window *RE_Window_new(char *title, int width, int height, int flags);

/// @ingroup RE_Window
/// @brief Détruit une fenêtre préalablement allouée avec RE_Window_new();
/// @param[in,out] window la fenêtre à détruire. 
extern RE_API void RE_Window_free(RE_Window * window);

/// @ingroup RE_Window
/// @brief Renvoie le moteur de rendu associé à la fenêtre.
/// @param[in] window la fenêtre.
/// @return Le moteur de rendu de la fenêtre.
extern RE_API RE_Renderer *RE_Window_getRenderer(RE_Window *window);

/// @ingroup RE_Window
/// @brief Initialise la librairie.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
extern RE_API int RE_init();

/// @ingroup RE_Window
/// @brief Quitte la librairie.
extern RE_API void RE_quit();

#endif