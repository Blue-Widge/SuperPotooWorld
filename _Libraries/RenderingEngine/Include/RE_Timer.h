
/// @file RE_Timer.h
/// @defgroup RE_Timer Fonctions relatives à la gestion du temps

#ifndef _RE_TIMER_H_
#define _RE_TIMER_H_

#include "RE_Lib.h"

/// @ingroup RE_Timer
/// @brief Structure représentant un chronomètre.
typedef struct RE_Timer_s
{
    /// @protected
    /// @brief Temps de départ.
    float m_startTime;

    /// @protected
    /// @brief Temps du dernier appel à RE_Timer_update().
    float m_currentTime;

    /// @protected
    /// @brief Temps de l'avant dernier appel à RE_Timer_update().
    float m_previousTime;

    /// @protected
    /// @brief Ecart entre les deux derniers appels à RE_Timer_update().
    float m_delta;
} RE_Timer;

/// @ingroup RE_Timer
/// @brief Crée un nouveau timer.
/// @return Le timer créé ou NULL en cas d'erreur.
extern RE_API RE_Timer* RE_Timer_new();

/// @ingroup RE_Timer
/// @brief Détruit un timer préalablement alloué avec RE_Timer_new().
/// @param[in,out] timer le timer à détruire.
extern RE_API void RE_Timer_free(RE_Timer* timer);

/// @ingroup RE_Timer
/// @brief Lance le timer.
/// @param[in,out] timer le timer.
extern RE_API void RE_Timer_start(RE_Timer* timer);

/// @ingroup RE_Timer
/// @brief Met à jour le timer en fonction du temps actuel.
/// Cette fonction est typiquement appelée à chaque tour de la boucle de rendu.
/// @param[in,out] timer le timer.
extern RE_API void RE_Timer_update(RE_Timer* timer);

/// @ingroup RE_Timer
/// @brief Renvoie l'écart de temps (en secondes) entre les deux derniers appels à la fonction
/// RE_Timer_update().
/// @param[in] timer le timer.
/// @return L'écart de temps entre les deux dernières mises à jour.
extern RE_API float RE_Timer_getDelta(RE_Timer *timer);

/// @ingroup RE_Timer
/// @brief Renvoie l'écart de temps (en secondes) entre le lancement du timer avec la fonction
/// RE_Timer_start() et le dernier appel à la fonction RE_Timer_update().
/// @param[in] timer le timer.
/// @return Le nombre de secondes écoulées depuis le lancement du timer et la dernière mise à jour.
extern RE_API float RE_Timer_getElapsed(RE_Timer *timer);

#endif