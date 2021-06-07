
/// @file PE_Trigger.h
/// @defgroup PE_Trigger Fonctions relatives à une détection

#ifndef _PE_TRIGGER_H_
#define _PE_TRIGGER_H_

#include "PE_Lib.h"
#include "PE_List.h"
#include "PE_Collider.h"

typedef struct PE_TriggerPair_s PE_TriggerPair;

/// @ingroup PE_Trigger
/// @brief Structure contenant les informations sur une détection entre deux colliders.
typedef struct PE_Trigger_s
{
    /// @protected
    /// @brief Noeud contenant un pointeur vers cette détection dans la listes des détections du collider parent.
    /// Ce membre appartient au collider parent.
    PE_Node m_colliderNode;

    /// @protected
    /// @brief Pointeur vers le collider principal impliqué dans la détection.
    PE_Collider *m_collider;

    /// @protected
    /// @brief Pointeur vers le second collider impliqué dans la détection.
    PE_Collider *m_otherCollider;

    /// @protected
    /// @brief Pointeur vers la paire de détection.
    /// Il s'agit de la structure contenant cette détection dans le détecteur de collisions.
    PE_TriggerPair *m_triggerPair;

} PE_Trigger;

PE_INLINE PE_TriggerPair *PE_Trigger_getTriggerPair(PE_Trigger *trigger)
{
    return trigger->m_triggerPair;
}

typedef enum PE_TriggerFlags_e
{
    PE_TRIGGER_OVERLAPPING = 1 << 0,
    PE_TRIGGER_TO_UPDATE   = 1 << 1,
    PE_TRIGGER_TO_REMOVE   = 1 << 2,
    PE_TRIGGER_CHECKED     = 1 << 3,
    PE_TRIGGER_STAY        = 1 << 4,
} PE_TriggerFlags;

typedef struct PE_TriggerPair_s
{
    /// @protected
    /// @brief The node which contains this trigger pair in the trigger list of the collision detector.
    PE_Node m_collisionDetectorNode;

    int m_flags;

    float m_timeLeft;

    /// @protected
    /// @brief Pointer to the first collider in collision.
    PE_Collider *m_colliderA;

    /// @protected
    /// @brief Pointer to the second collider in collision.
    PE_Collider *m_colliderB;

    /// @protected
    /// @brief Trigger for the first collider.
    PE_Trigger m_triggerA;

    /// @protected
    /// @brief Trigger for the second collider.
    PE_Trigger m_triggerB;

} PE_TriggerPair;

PE_TriggerPair *PE_TriggerPair_new(PE_Collider *colliderA, PE_Collider *colliderB, PE_Allocator *allocator);
void PE_TriggerPair_free(PE_TriggerPair *triggerPair, PE_Allocator *allocator);

PE_INLINE int PE_TriggerPair_getFlags(PE_TriggerPair *triggerPair)
{
    return triggerPair->m_flags;
}

PE_INLINE Bool PE_TriggerPair_hasOneFlag(PE_TriggerPair *triggerPair, int flags)
{
    return (triggerPair->m_flags & flags);
}

PE_INLINE void PE_TriggerPair_addFlags(PE_TriggerPair *triggerPair, int flags)
{
    triggerPair->m_flags |= flags;
}

PE_INLINE void PE_TriggerPair_removeFlags(PE_TriggerPair *triggerPair, int flags)
{
    triggerPair->m_flags &= ~flags;
}

PE_INLINE void PE_TriggerPair_setTimeLeft(PE_TriggerPair *triggerPair, float timeLeft)
{
    triggerPair->m_timeLeft = timeLeft;
}

PE_INLINE float PE_TriggerPair_getTimeLeft(PE_TriggerPair *triggerPair)
{
    return triggerPair->m_timeLeft;
}

PE_INLINE float PE_TriggerPair_addToTimeLeft(PE_TriggerPair *triggerPair, float delta)
{
    return triggerPair->m_timeLeft += delta;
}

void PE_TriggerPair_update(PE_TriggerPair *triggerPair);


// TRIGGER USER METHODS

/// @ingroup PE_Trigger
/// @brief Renvoie le collider principal impliqué dans une détection.
/// @param[in] trigger la détection.
/// @return Le collider principal impliqué dans la détection.
PE_API PE_INLINE PE_Collider *PE_Trigger_getCollider(PE_Trigger *trigger)
{
    return trigger->m_collider;
}

/// @ingroup PE_Trigger
/// @brief Renvoie le second collider impliqué dans une détection.
/// @param[in] trigger la détection.
/// @return Le second collider impliqué dans la détection.
PE_API PE_INLINE PE_Collider *PE_Trigger_getOtherCollider(PE_Trigger *trigger)
{
    return trigger->m_otherCollider;
}

/// @ingroup PE_Trigger
/// @brief Renvoie le corps principal impliqué dans une détection.
/// @param[in] trigger la détection.
/// @return Le corps principal impliqué dans la détection.
PE_API PE_INLINE PE_Body *PE_Trigger_getBody(PE_Trigger *trigger)
{
    return PE_Collider_getBody(trigger->m_collider);
}

/// @ingroup PE_Trigger
/// @brief Renvoie le second corps impliqué dans une détection.
/// @param[in] trigger la détection.
/// @return Le second corps impliqué dans la détection.
PE_API PE_INLINE PE_Body *PE_Trigger_getOtherBody(PE_Trigger *trigger)
{
    return PE_Collider_getBody(trigger->m_otherCollider);
}

#endif