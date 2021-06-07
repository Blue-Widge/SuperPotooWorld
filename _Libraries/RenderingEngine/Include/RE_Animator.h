
/// @file RE_Animator.h
/// @defgroup RE_Animator Fonctions relatives à un animateur

#ifndef _RE_ANIMATOR_H_
#define _RE_ANIMATOR_H_

#include "RE_Lib.h"
#include "RE_Texture.h"
#include "RE_Timer.h"
#include "RE_Renderer.h"
#include "RE_Utils.h"
#include "RE_HashTable.h"

/// @ingroup RE_Animator
/// @brief Type d'une courbe d'accélération.
/// Ces fonctions sont définies sur [0,1] et vérifient généralement f(0) = 0 et f(1) = 1.
typedef float (*RE_EasingFct)(float);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération linéaire.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
extern RE_API float RE_EasingFct_linear(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération douce en entrée.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
extern RE_API float RE_EasingFct_in(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération douce en sortie.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
extern RE_API float RE_EasingFct_out(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération avec douce en entrée et en sortie.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
extern RE_API float RE_EasingFct_inOut(float x);

/// @ingroup RE_Animator
/// @brief Courbe d'accélération définie à partir du cosinus.
/// @param[in] x ratio entre 0.f et 1.f.
/// @return Le ratio de progression entre 0.f et 1.f.
extern RE_API float RE_EasingFct_cos(float x);

/// @ingroup RE_Animator
/// @brief Enumération contenant les flags pouvant être utilisés
/// pour contrôler l'animation d'une texture ou d'un paramètre.
typedef enum RE_AnimFlags_e
{
    /// @brief Met l'animation en pause.
    RE_ANIM_PAUSED    = 1 << 1,
    /// @brief Joue l'animation dans le sens inverse.
    RE_ANIM_REVERSED  = 1 << 2,
    /// @brief Joue l'animation en avant puis en arrière.
    RE_ANIM_ALTERNATE = 1 << 3
} RE_AnimFlags;

/// @ingroup RE_Animator
/// @brief Structure représentant une texture animée.
typedef struct RE_TextureAnim_s
{
    /// @protected
    /// @brief La texture.
    RE_Texture *m_texture;

    /// @protected
    /// @brief Le coefficient multiplicateur de la vitesse de lecture.
    float m_speed;

    /// @protected
    /// @brief La durée d'un cycle.
    float m_cycleTime;

    /// @protected
    /// @brief Délai avant le début de l'animation.
    float m_delay;

    /// @protected
    /// @brief Accumulateur.
    float m_accu;

    /// @protected
    /// @brief Accumulateur pour le délai.
    float m_delayAccu;

    /// @protected
    /// @brief Les flags de l'animation.
    /// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
    int m_flags;

    /// @protected
    /// @brief Nombre de cycles de l'animation.
    /// L'animation tourne en boucle si ce membre vaut -1.
    int m_nbCycles;

    /// @protected
    /// @brief Indice du cycle courant. 
    int m_cycleIdx;

    /// @protected
    /// @brief Nombre de sous-images dans l'atlas de texture.
    int m_nbParts;
} RE_TextureAnim;

/// @ingroup RE_Animator
/// @brief Définit la durée d'un cycle de l'animation d'une texture.
/// @param[in,out] textureAnim la texture animée.
/// @param[in] cycleTime la durée d'un cycle.
RE_API RE_INLINE void RE_TextureAnim_setCycleTime(RE_TextureAnim *textureAnim, float cycleTime)
{
    textureAnim->m_cycleTime = cycleTime;
}

/// @ingroup RE_Animator
/// @brief Définit le nombre de cycle de l'animation d'une texture.
/// L'animation se répète indéfiniment si le nombre de cycle vaut -1.
/// @param[in,out] textureAnim la texture animée.
/// @param[in] nbCycles le nombre de cycle de l'animation.
RE_API RE_INLINE void RE_TextureAnim_setNbCyles(RE_TextureAnim *textureAnim, int nbCycles)
{
    textureAnim->m_nbCycles = nbCycles;
    textureAnim->m_cycleIdx = 0;
}

/// @ingroup RE_Animator
/// @brief Définit le délai avant le début de l'animation d'une texture.
/// @param[in,out] textureAnim la texture animée.
/// @param[in] delay le délai de l'animation.
RE_API RE_INLINE void RE_TextureAnim_setDelay(RE_TextureAnim *textureAnim, float delay)
{
    textureAnim->m_delay = delay;
}

/// @ingroup RE_Animator
/// @brief Définit la vitesse de lecture d'une texture animée. La valeur par défaut est 1.f.
/// Si elle vaut par exemple 2.f, l'animation se déroulera deux fois plus vite.
/// @param[in,out] textureAnim la texture animée.
/// @param[in] speed coefficient multiplicateur de la vitesse de lecture.
RE_API RE_INLINE void RE_TextureAnim_setSpeed(RE_TextureAnim *textureAnim, float speed)
{
    textureAnim->m_speed = speed;
}

/// @ingroup RE_Animator
/// @brief Définit les flags d'une texture animée.
/// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
/// @param[in,out] textureAnim la texture animée.
/// @param[in] flags les flags de l'animation.
RE_API RE_INLINE void RE_TextureAnim_setFlags(RE_TextureAnim *textureAnim, int flags)
{
    textureAnim->m_flags = flags;
}

/// @ingroup RE_Animator
/// @brief Ajoute des flags à une texture animée.
/// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
/// @param[in,out] textureAnim la texture animée.
/// @param[in] flags les flags à ajouter.
RE_API RE_INLINE void RE_TextureAnim_addFlags(RE_TextureAnim *textureAnim, int flags)
{
    textureAnim->m_flags |= flags;
}

/// @ingroup RE_Animator
/// @brief Enlève des flags à une texture animée.
/// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
/// @param[in,out] textureAnim la texture animée.
/// @param[in] flags les flags à enlever.
RE_API RE_INLINE void RE_TextureAnim_removeFlags(RE_TextureAnim *textureAnim, int flags)
{
    textureAnim->m_flags &= ~flags;
}

/// @ingroup RE_Animator
/// @brief Enumération contenant les paramètres pouvant êtres animés par un RE_Animator.
typedef enum RE_AnimableParam_e
{
    RE_PARAM_NONE = 0,
    RE_PARAM_ALPHA,
    RE_PARAM_SHIFT,
    RE_PARAM_ANGLE,
} RE_AnimableParam;

/// @ingroup RE_Animator
/// @brief Structure représentant un paramètre animé.
typedef struct RE_ParamAnim_s
{
    /// @protected
    /// @brief Le paramètre animé.
    RE_AnimableParam m_parameter;

    /// @protected
    /// @brief Courbe d'accélération de l'animation.
    RE_EasingFct m_easing;

    /// @protected
    /// @brief Phase de l'animation.
    float m_phase;

    /// @protected
    /// @brief Le coefficient multiplicateur de la vitesse de lecture.
    float m_speed;

    /// @protected
    /// @brief La durée d'un cycle.
    float m_cycleTime;

    /// @protected
    /// @brief Délai avant le début de l'animation.
    float m_delay;

    /// @protected
    /// @brief Accumulateur.
    float m_accu;

    /// @protected
    /// @brief Accumulateur pour le délai.
    float m_delayAccu;

    /// @protected
    /// @brief Les flags de l'animation.
    /// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
    int m_flags;

    /// @protected
    /// @brief Nombre de cycles de l'animation.
    /// L'animation tourne en boucle si ce membre vaut -1.
    int m_nbCycles;

    /// @protected
    /// @brief Indice du cycle courant. 
    int m_cycleIdx;

    /// @protected
    /// @brief Valeurs initiale et finale du paramètre.
    union {
        /// @brief Valeurs de l'opacité.
        int   alpha[2];
        /// @brief Valeurs du décalage.
        Vec2  shift[2];
        /// @brief Valeurs de l'angle de rotation.
        float angle[2];
    } m_values;
} RE_ParamAnim;

/// @ingroup RE_Animator
/// @brief Définit le paramètre animé comme étant une variation de position
/// et spécifie ses valeurs initiale et finale.
/// @param[in,out] paramAnim la paramètre animé.
/// @param[in] shift0 la valeur initiale du décalage.
/// @param[in] shift1 la valeur finale du décalage.
RE_API RE_INLINE void RE_ParamAnim_setShift(RE_ParamAnim *paramAnim, Vec2 shift0, Vec2 shift1)
{
    paramAnim->m_parameter = RE_PARAM_SHIFT;
    paramAnim->m_values.shift[0] = shift0;
    paramAnim->m_values.shift[1] = shift1;
}

/// @ingroup RE_Animator
/// @brief Définit le paramètre animé comme étant une variation d'opacité
/// et spécifie ses valeurs initiale et finale.
/// @param[in,out] paramAnim la paramètre animé.
/// @param alpha0 la valeur initiale de l'opacité.
/// @param alpha1 la valeur finale de l'opacité.
RE_API RE_INLINE void RE_ParamAnim_setAlpha(RE_ParamAnim *paramAnim, int alpha0, int alpha1)
{
    paramAnim->m_parameter = RE_PARAM_ALPHA;
    paramAnim->m_values.alpha[0] = alpha0;
    paramAnim->m_values.alpha[1] = alpha1;
}

/// @ingroup RE_Animator
/// @brief Définit le paramètre animé comme étant une variation de l'angle de rotation
/// et spécifie ses valeurs initiale et finale.
/// @param[in,out] paramAnim la paramètre animé.
/// @param angle0 la valeur initiale de l'angle de rotation.
/// @param angle1 la valeur finale de l'angle de rotation.
RE_API RE_INLINE void RE_ParamAnim_setAngle(RE_ParamAnim *paramAnim, float angle0, float angle1)
{
    paramAnim->m_parameter = RE_PARAM_ANGLE;
    paramAnim->m_values.angle[0] = angle0;
    paramAnim->m_values.angle[1] = angle1;
}

/// @ingroup RE_Animator
/// @brief Définit la durée d'un cycle de l'animation d'un paramètre.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] cycleTime la durée d'un cycle.
RE_API RE_INLINE void RE_ParamAnim_setCycleTime(RE_ParamAnim *paramAnim, float cycleTime)
{
    paramAnim->m_cycleTime = cycleTime;
}

/// @ingroup RE_Animator
/// @brief Définit le nombre de cycle de l'animation d'un paramètre.
/// L'animation se répète indéfiniment si le nombre de cycle vaut -1.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] nbCycles le nombre de cycle de l'animation.
RE_API RE_INLINE void RE_ParamAnim_setNbCyles(RE_ParamAnim *paramAnim, int nbCycles)
{
    paramAnim->m_nbCycles = nbCycles;
    paramAnim->m_cycleIdx = 0;
}

/// @ingroup RE_Animator
/// @brief Définit le délai avant le début de l'animation d'un paramètre.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] delay le délai de l'animation.
RE_API RE_INLINE void RE_ParamAnim_setDelay(RE_ParamAnim *paramAnim, float delay)
{
    paramAnim->m_delay = delay;
}

/// @ingroup RE_Animator
/// @brief Définit la courbe d'accélération de l'animation d'un paramètre.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] easing la courbe d'accélération.
RE_API RE_INLINE void RE_ParamAnim_setEasing(RE_ParamAnim *paramAnim, RE_EasingFct easing)
{
    paramAnim->m_easing = easing;
}

/// @ingroup RE_Animator
/// @brief Définit la phase de l'animation d'un paramètre.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] phase la phase (dans l'interval [0,1]).
RE_API RE_INLINE void RE_ParamAnim_setPhase(RE_ParamAnim *paramAnim, float phase)
{
    paramAnim->m_phase = phase;
}

/// @ingroup RE_Animator
/// @brief Définit la vitesse de lecture d'un paramètre animé. La valeur par défaut est 1.f.
/// Si elle vaut par exemple 2.f, l'animation se déroulera deux fois plus vite.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] speed coefficient multiplicateur de la vitesse de lecture.
RE_API RE_INLINE void RE_ParamAnim_setSpeed(RE_ParamAnim *paramAnim, float speed)
{
    paramAnim->m_speed = speed;
}

/// @ingroup RE_Animator
/// @brief Définit les flags d'un paramètre animé.
/// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] flags les flags de l'animation.
RE_API RE_INLINE void RE_ParamAnim_setFlags(RE_ParamAnim *paramAnim, int flags)
{
    paramAnim->m_flags = flags;
}

/// @ingroup RE_Animator
/// @brief Ajoute des flags à un paramètre animé.
/// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] flags les flags à ajouter.
RE_API RE_INLINE void RE_ParamAnim_addFlags(RE_ParamAnim *paramAnim, int flags)
{
    paramAnim->m_flags |= flags;
}

/// @ingroup RE_Animator
/// @brief Enlève des flags à un paramètre animé.
/// Les valeurs possibles des flags sont données dans l'énumération RE_AnimFlags.
/// @param[in,out] paramAnim le paramètre animé.
/// @param[in] flags les flags à enlever.
RE_API RE_INLINE void RE_ParamAnim_removeFlags(RE_ParamAnim *paramAnim, int flags)
{
    paramAnim->m_flags &= ~flags;
}

/// @ingroup RE_Animator
/// @brief Structure représentant un animateur.
/// Cela permet d'animer un ensemble de plusieurs atlas de textures
/// et d'y ajouter des effets comme des variations de position ou d'opacité.
typedef struct RE_Animator_s
{
    /// @protected
    /// @brief Table de hachage contenant les textures animées.
    RE_HashTable *m_texAnimsHT;

    /// @protected
    /// @brief Table de hachage contenant les paramètres animés.
    RE_HashTable *m_parAnimsHT;

    /// @protected
    /// @brief Table de hachage contenant les paramètres animés actifs.
    RE_HashTable *m_activeParAnimsHT;

    /// @protected
    /// @brief Pointeur vers la texture animée active.
    RE_TextureAnim *m_activeTexAnim;

    /// @protected
    /// @brief Transformation à appliquer à la texture au moment du rendu.
     RE_Transform m_transform;

     /// @protected
    /// @brief Indice de la partie à rendre dans l'atlas de texture actif.
    int m_partIdx;

    /// @protected
    /// @brief Décalage de la position du rendu.
    Vec2 m_shift;

    /// @protected
    /// @brief Opacité du rendu.
    int m_alpha;
} RE_Animator;

/// @ingroup RE_Animator
/// @brief Crée un nouvel animateur.
/// @return L'animateur créé ou NULL en cas d'erreur.
extern RE_API RE_Animator *RE_Animator_new();

/// @ingroup RE_Animator
/// @brief Détruit un animateur préalablement créé avec RE_Animator_new().
/// Détruit également toutes les animations contenues dans l'animateur.
/// Ne libère pas les textures associées.
/// @param[in,out] animator l'animateur à détruire.
extern RE_API void RE_Animator_free(RE_Animator *animator);

/// @ingroup RE_Animator
/// @brief Ajoute une texture animée à un animateur.
/// L'animation est construite à partir d'une texture (ou plus généralement un atlas de texture).
/// La fonction renvoie un pointeur sur l'animation crée pour pouvoir la paramétrer.
/// La fonction RE_Animator_playTextureAnim() permet ensuite de lancer l'animation en utilisant son nom.
/// @param[in,out] animator l'animateur.
/// @param[in] texture la texture contenant les différentes étapes de l'animation.
/// @param[in] name le nom de l'animation.
/// @return La texture animée créée ou NULL en cas d'erreur.
extern RE_API RE_TextureAnim *RE_Animator_createTextureAnim(RE_Animator *animator, RE_Texture *texture, char *name);

/// @ingroup RE_Animator
/// @brief Ajoute un paramètre animé à un animateur.
/// La fonction renvoie un pointeur sur l'animation crée pour pouvoir la paramétrer.
/// La fonction RE_Animator_playParamAnim() permet ensuite de lancer l'animation en utilisant son nom.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return Le paramètre animé créé ou NULL en cas d'erreur.
extern RE_API RE_ParamAnim *RE_Animator_createParamAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Renvoie un pointeur sur une texture animée d'un animateur à partir de son nom.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return Un pointeur vers la texture animée
/// ou NULL s'il n'y a aucune animation correspondant au nom donné.
extern RE_API RE_TextureAnim *RE_Animator_getTextureAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Renvoie un pointeur sur un paramètre animé d'un animateur à partir de son nom.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return Un pointeur vers le paramètre animé
/// ou NULL s'il n'y a aucune animation correspondant au nom donné.
extern RE_API RE_ParamAnim *RE_Animator_getParamAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Détruit une texture animée d'un animateur.
/// Cette fonction ne libère pas la texture associée.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation correspondant au nom donné.
extern RE_API int RE_Animator_destroyTextureAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Détruit un paramètre animé d'un animateur.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation correspondant au nom donné.
extern RE_API int RE_Animator_destroyParamAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Lance l'animation d'une texture d'un animateur.
/// Si l'animateur joue déjà une animation, elle est remplacée par la nouvelle.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation correspondant au nom donné.
extern RE_API int RE_Animator_playTextureAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Lance l'animation d'un paramètre d'un animateur.
/// Le paramètre s'ajoute aux paramètres actifs et ne les remplace pas.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation correspondant au nom donné.
extern RE_API int RE_Animator_playParamAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Reprend l'animation d'une texture d'un animateur.
/// Si l'animateur joue déjà une animation, elle est remplacée par la nouvelle.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation correspondant au nom donné.
extern RE_API int RE_Animator_resumeTextureAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Reprend l'animation d'un paramètre d'un animateur.
/// Le paramètre s'ajoute aux paramètres actifs et ne les remplace pas.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation correspondant au nom donné.
extern RE_API int RE_Animator_resumeParamAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Arrète l'animation de la texture animée courante d'un animateur.
/// @param[in,out] animator l'animateur.
extern RE_API void RE_Animator_stopTextureAnim(RE_Animator *animator);

/// @ingroup RE_Animator
/// @brief Arrète l'animation d'un paramètre animé actif d'un animateur.
/// @param[in,out] animator l'animateur.
/// @param[in] name le nom de l'animation.
/// @return EXIT_SUCCESS ou EXIT_FAILURE
/// s'il n'y a aucune animation active correspondant au nom donné.
extern RE_API int RE_Animator_stopParamAnim(RE_Animator *animator, char *name);

/// @ingroup RE_Animator
/// @brief Arrète toutes les animations des paramètres animés actifs d'un animateur.
/// @param[in,out] animator l'animateur.
extern RE_API void RE_Animator_stopParamAnims(RE_Animator *animator);

/// @ingroup RE_Animator
/// @brief Définit la composante alpha (transparence) du rendu.
/// Ce paramètre est écrasé par une animation de l'opacité.
/// @param[in,out] animator l'animateur.
/// @param[in] alpha la composante alpha.
RE_API RE_INLINE void RE_Animator_setAlpha(RE_Animator *animator, int alpha)
{
    animator->m_alpha = alpha;
}

/// @ingroup RE_Animator
/// @brief Définit la transformation à appliquer sur le rendu.
/// @param[in,out] animator l'animateur.
/// @param[in] transform la transformation à appliquer.
RE_API RE_INLINE void RE_Animator_setTransform(RE_Animator *animator, RE_Transform *transform)
{
    animator->m_transform = *transform;
}

/// @ingroup RE_Animator
/// @brief Renvoie la transformation appliquée sur le rendu.
/// @param[in] animator l'animateur.
/// @param[out] transform la transformation appliquée sur le rendu.
RE_API RE_INLINE void RE_Animator_getTransform(RE_Animator *animator, RE_Transform *transform)
{
    *transform = animator->m_transform;
}

/// @ingroup RE_Animator
/// @brief Dessine l'image courante de l'animation dans son contexte de rendu.
/// @param[in,out] animator l'animateur.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
extern RE_API void RE_Animator_render(RE_Animator *animator, int x, int y);

/// @ingroup RE_Animator
/// @brief Dessine l'image courante de l'animation dans son contexte de rendu.
/// Contrairement à RE_Animator_render(), cette fonction effectue le rendu avec une précision en virgule flottante.
/// @param[in,out] animator l'animateur.
/// @param[in] x abscisse du point (en pixels) où dessiner la texture.
/// @param[in] y ordonnée du point (en pixels) où dessiner la texture.
extern RE_API void RE_Animator_renderF(RE_Animator *animator, float x, float y);

/// @ingroup RE_Animator
/// @brief Met à jour l'animateur en fonction du temps écoulé.
/// @param[in,out] animator l'animateur.
/// @param[in] timer le timer principal de la boucle de rendu.
extern RE_API void RE_Animator_update(RE_Animator *animator, RE_Timer *timer);

#endif
