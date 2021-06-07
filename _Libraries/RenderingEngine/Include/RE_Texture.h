
/// @file RE_Texture.h
/// @defgroup RE_Texture Fonctions relatives à une texture

#ifndef _RE_TEXTURE_H_
#define _RE_TEXTURE_H_

#include "RE_Lib.h"
#include "RE_Renderer.h"
#include "RE_Utils.h"

#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

/// @ingroup RE_Texture
/// @brief Structure représentant une image optimisée pour un moteur de rendu.
/// La texture peut contenir plusieurs sous-images organisées sous forme d'un tableau 2D,
/// il s'agit alors d'un atlas de texture.
/// Les différentes parties d'un atlas sont indicées à partir de 0 et dans l'ordre naturel de lecture.
typedef struct RE_Texture_s
{
    /// @protected
    /// @brief Le moteur de rendu associé à la texture.
    RE_Renderer *m_renderer;

    /// @protected
    /// @brief La texture SDL.
    SDL_Texture *m_textureSDL;

    /// @protected
    /// @brief La largeur en pixels de la texture.
    int m_width;

    /// @protected
    /// @brief La hauteur en pixels de la texture.
    int m_height;

    /// @protected
    /// @brief Nombre de lignes de l'atlas de texture.
    int m_nbRows;

    /// @protected
    /// @brief Nombre de colonnes de l'atlas de texture.
    int m_nbCols;

    /// @protected
    /// @brief Nombre de sous-images dans l'atlas de texture.
    /// Correspond à m_nbRows * m_nbCols.
    int m_nbParts;

    /// @protected
    /// @brief Tableau contenant les coordonnées de texture des différentes partie de l'atlas de texture.
    SDL_Rect *m_parts;
} RE_Texture;

/// @ingroup RE_Texture
/// @brief Enumération contenant les flags pouvant être utilisés pour retourner un texture au moment du rendu.
typedef enum RE_TextureFlip_e
{
    /// @brief Aucun retournement.
    RE_FLIP_NONE       = SDL_FLIP_NONE,
    /// @brief Retournement horizontal.
    RE_FLIP_HORIZONTAL = SDL_FLIP_HORIZONTAL,
    /// @brief Retournement Vertical.
    RE_FLIP_VERTICAL   = SDL_FLIP_VERTICAL
} RE_TextureFlip;

/// @ingroup RE_Texture
/// @brief Structure représentant la transformation à appliquer à une texture au moment du rendu.
typedef struct RE_Transform_s
{
    /// @brief L'angle de la rotation.
    float angle;
    /// @brief Le centre de la rotation en coordonnées de texture.
    Vec2 *center;
    /// @brief Les flags de retournement.
    /// Les valeurs possibles des flags sont données dans l'énumération RE_TextureFlip.
    int flipFlags;
} RE_Transform;

/// @ingroup RE_Texture
/// @brief Renvoie la transformation par défaut.
/// Elle est paramétrée pour qu'aucun changement ne soit appliqué à l'image.
/// @return La tranformation par défaut.
extern RE_API RE_Transform RE_Transform_getDefault();

/// @ingroup RE_Texture
/// @brief Crée une nouvelle texture.
/// @param[in] path le chemin d'accès à la texture.
/// @param[in,out] renderer le moteur de rendu.
/// @return La texture créée ou NULL en cas d'erreur.
extern RE_API RE_Texture *RE_Texture_new(char *path, RE_Renderer *renderer);

/// @ingroup RE_Texture
/// @brief Crée un nouvel atlas de texture composé de plusieurs sous-images de mêmes dimensions
/// organisées sous forme d'un tableau 2D.
/// @param[in] path le chemin d'accès à la texture.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] nbRows le nombre de lignes de l'atlas.
/// @param[in] nbCols le nombre de colonnes de l'atlas.
/// @return La texture créée ou NULL en cas d'erreur.
extern RE_API RE_Texture *RE_Texture_newSet(char *path, RE_Renderer *renderer, int nbRows, int nbCols);

/// @ingroup RE_Texture
/// @brief Détruit une texture préalablement créée avec RE_Texture_new() ou RE_Texture_newSet().
/// @param[in,out] texture la texture à détruire.
extern RE_API void RE_Texture_free(RE_Texture *texture);

/// @ingroup RE_Texture
/// @brief Renvoie la largeur totale d'une texture.
/// @param[in] texture la texture.
/// @return La largeur totale de la texture.
RE_API RE_INLINE int RE_Texture_getWidth(RE_Texture *texture)
{
    return texture->m_width;
}

/// @ingroup RE_Texture
/// @brief Renvoie la hauteur totale d'une texture.
/// @param[in] texture la texture.
/// @return La hauteur totale de la texture.
RE_API RE_INLINE int RE_Texture_getHeight(RE_Texture *texture)
{
    return texture->m_height;
}

/// @ingroup RE_Texture
/// @brief Renvoie le nombre de sous-images dans l'atlas de texture.
/// Dans le cas d'une texture simple en contenant pas de sous-images, cette fonction renvoie 1.
/// @param[in] texture la texture.
/// @return Le nombre de sous-images dans l'atlas de texture.
RE_API RE_INLINE int RE_Texture_getNbParts(RE_Texture *texture)
{
    return texture->m_nbParts;
}

/// @ingroup RE_Texture
/// @brief Renvoie la largeur d'une sous-image d'un atlas de texture.
/// Dans le cas d'une texture simple en contenant pas de sous-images,
/// cette fonction est identique à RE_Texture_getWidth()
/// @param[in] texture la texture.
/// @return La largeur d'une sous-image de l'atlas de texture.
RE_API RE_INLINE int RE_Texture_getPartWidth(RE_Texture *texture, int partIdx)
{
    return (texture->m_parts[partIdx]).w;
}

/// @ingroup RE_Texture
/// @brief Renvoie la hauteur d'une sous-image d'un atlas de texture.
/// Dans le cas d'une texture simple en contenant pas de sous-images,
/// cette fonction est identique à RE_Texture_getHeight()
/// @param[in] texture la texture.
/// @return La hauteur d'une sous-image de l'atlas de texture.
RE_API RE_INLINE int RE_Texture_getPartHeight(RE_Texture *texture, int partIdx)
{
    return (texture->m_parts[partIdx]).h;
}

/// @ingroup RE_Texture
/// @brief Définit la composante alpha (transparence) d'une texture.
/// @param[in,out] texture la texture.
/// @param[in] alpha la composante alpha.
RE_API RE_INLINE void RE_Texture_setAlpha(RE_Texture *texture, int alpha)
{
    SDL_SetTextureAlphaMod(texture->m_textureSDL, (Uint8)alpha);
}

/// @ingroup RE_Texture
/// @brief Renvoie la composante alpha (transparence) d'une texture.
/// @param[in] texture la texture.
/// @return La composante alpha de la texture.
RE_API RE_INLINE int RE_Texture_getAlpha(RE_Texture *texture)
{
    Uint8 alpha;
    SDL_GetTextureAlphaMod(texture->m_textureSDL, &alpha);
    return (int)alpha;
}

/// @ingroup RE_Texture
/// @brief Dessine une partie d'une texture dans son contexte de rendu.
/// @param[in] texture la texture ou l'atlas de texture.
/// @param[in] partIdx l'indice de la partie à dessiner. Cet argument doit être égal à 0 dans le cas d'une texture simple.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
extern RE_API void RE_Texture_render(RE_Texture *texture, int partIdx, int x, int y);

/// @ingroup RE_Texture
/// @brief Dessine une partie d'une texture dans son contexte de rendu avec la possibilité de la retourner.
/// @param[in] texture la texture ou l'atlas de texture.
/// @param[in] partIdx l'indice de la partie à dessiner. Cet argument doit être égal à 0 dans le cas d'une texture simple.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
/// @param[in] flipFlags les flags spécifiant le retournement à appliquer.
///       Les valeurs possibles des flags sont données dans l'énumération RE_TextureFlip.
extern RE_API void RE_Texture_renderFlip(RE_Texture *texture, int partIdx, int x, int y, int flipFlags);

/// @ingroup RE_Texture
/// @brief Dessine une partie d'une texture dans son contexte de rendu avec la possibilité de lui appliquer une rotation.
/// @param[in] texture la texture ou l'atlas de texture.
/// @param[in] partIdx l'indice de la partie à dessiner. Cet argument doit être égal à 0 dans le cas d'une texture simple.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
/// @param[in] angle l'angle de la rotation à appliquer.
/// @param[in] center le centre de la rotation à appliquer.
extern RE_API void RE_Texture_renderRotate(RE_Texture *texture, int partIdx, int x, int y, float angle, Vec2 *center);

/// @ingroup RE_Texture
/// @brief Dessine une partie d'une texture dans son contexte de rendu avec la possibilité de lui appliquer une transformation.
/// @param[in] texture la texture ou l'atlas de texture.
/// @param[in] partIdx l'indice de la partie à dessiner. Cet argument doit être égal à 0 dans le cas d'une texture simple.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
/// @param[in] transform la transformation à appliquer.
extern RE_API void RE_Texture_renderTransform(RE_Texture *texture, int partIdx, int x, int y, RE_Transform *transform);

/// @ingroup RE_Texture
/// @brief Dessine une partie d'une texture dans son contexte de rendu.
/// Contrairement à RE_Texture_render(), cette fonction effectue le rendu avec une précision en virgule flottante.
/// @param[in] texture la texture ou l'atlas de texture.
/// @param[in] partIdx l'indice de la partie à dessiner. Cet argument doit être égal à 0 dans le cas d'une texture simple.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
extern RE_API void RE_Texture_renderF(RE_Texture *texture, int partIdx, float x, float y);

/// @ingroup RE_Texture
/// @brief Dessine une partie d'une texture dans son contexte de rendu avec la possibilité de la retourner.
/// Contrairement à RE_Texture_renderFlip(), cette fonction effectue le rendu avec une précision en virgule flottante.
/// @param[in] texture la texture ou l'atlas de texture.
/// @param[in] partIdx l'indice de la partie à dessiner. Cet argument doit être égal à 0 dans le cas d'une texture simple.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
/// @param[in] flipFlags les flags spécifiant le retournement à appliquer.
///       Les valeurs possibles des flags sont données dans l'énumération RE_TextureFlip.
extern RE_API void RE_Texture_renderFlipF(RE_Texture *texture, int partIdx, float x, float y, int flipFlags);

/// @ingroup RE_Texture
/// @brief Dessine une partie d'une texture dans son contexte de rendu avec la possibilité de lui appliquer une rotation.
/// Contrairement à RE_Texture_renderRotate(), cette fonction effectue le rendu avec une précision en virgule flottante.
/// @param[in] texture la texture ou l'atlas de texture.
/// @param[in] partIdx l'indice de la partie à dessiner. Cet argument doit être égal à 0 dans le cas d'une texture simple.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
/// @param[in] angle l'angle de la rotation à appliquer.
/// @param[in] center le centre de la rotation à appliquer.
extern RE_API void RE_Texture_renderRotateF(RE_Texture *texture, int partIdx, float x, float y, float angle, Vec2 *center);

/// @ingroup RE_Texture
/// @brief Dessine une partie d'une texture dans son contexte de rendu avec la possibilité de lui appliquer une transformation.
/// Contrairement à RE_Texture_renderTransform(), cette fonction effectue le rendu avec une précision en virgule flottante.
/// @param[in] texture la texture ou l'atlas de texture.
/// @param[in] partIdx l'indice de la partie à dessiner. Cet argument doit être égal à 0 dans le cas d'une texture simple.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
/// @param[in] transform la transformation à appliquer.
extern RE_API void RE_Texture_renderTransformF(RE_Texture *texture, int partIdx, float x, float y, RE_Transform *transform);

#endif
