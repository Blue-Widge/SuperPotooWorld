
/// @file PE_Collision.h
/// @defgroup PE_Collision Fonctions relatives à une collision

#ifndef _PE_COLLISION_H_
#define _PE_COLLISION_H_

#include "PE_Lib.h"
#include "PE_Math.h"
#include "PE_Collider.h"
#include "PE_List.h"
#include "PE_World.h"
#include "PE_Shape.h"
#include "PE_Manifold.h"

typedef struct PE_CollisionPair_s PE_CollisionPair;
typedef struct PE_SolverCollisionPair_s PE_SolverCollisionPair;

/// @ingroup PE_Collision
/// @brief Enumération donnant les positions relatives possible d'un collider par rapport à un autre.
typedef enum PE_RelativePosition_e
{
    PE_ABOVE = 0,
    PE_LEFT  = 1,
    PE_BELOW = 2,
    PE_RIGHT = 3
} PE_RelativePosition;

int PE_RelativePosition_compute(PE_Vec2 *normal);

PE_INLINE int PE_RelativePosition_getOpposite(PE_RelativePosition relPos)
{
    return relPos ^ 0x2;
}

/// @ingroup PE_Collision
/// @brief Structure contenant les informations sur une collision entre deux colliders.
typedef struct PE_Collision_s
{
    /// @protected
    /// @brief Noeud contenant un pointeur vers cette collision dans la listes des collisions du collider parent.
    /// Ce membre appartient au collider parent.
    PE_Node m_colliderNode;

    /// @protected
    /// @brief Pointeur vers le collider principal impliqué dans la collision.
    PE_Collider *m_collider;

    /// @protected
    /// @brief Pointeur vers le second collider impliqué dans la collision.
    PE_Collider *m_otherCollider;

    /// @protected
    /// @brief Le manifold de la collision.
    PE_Manifold m_manifold;

    /// @protected
    /// @brief La position relative du collider principal par rapport au second collider.
    PE_RelativePosition m_relPos;

    /// @protected
    /// @brief Pointeur vers la paire de collisions.
    /// Il s'agit de la structure contenant cette collision dans le détecteur de collisions.
    PE_CollisionPair *m_collisionPair;
} PE_Collision;

PE_INLINE PE_CollisionPair *PE_Collision_getCollisionPair(PE_Collision *collision)
{
    return collision->m_collisionPair;
}

typedef enum PE_CollisionFlag_e
{
    PE_COLLISION_TOUCHING    = 1 << 0,
    PE_COLLISION_OVERLAPPING = 1 << 1,
    PE_COLLISION_TO_UPDATE   = 1 << 2, // Indication suite un deplacement
    PE_COLLISION_TO_REMOVE   = 1 << 3, // Indication de la broad phase
    PE_COLLISION_CHECKED     = 1 << 4, // Collision validée par le solver
    PE_COLLISION_STAY        = 1 << 5,
    PE_COLLISION_IN_ISLAND   = 1 << 6,
} PE_CollisionFlag;

typedef struct PE_CollisionPair_s
{
    /// @protected
    /// @brief Belongs to the PE_CollisionDetector.
    /// The node which contains this collision in the collision list of the collision detector.
    PE_Node m_collisionDetectorNode;

    int m_flags;

    /// @protected
    /// @brief The time of impact between the two colliders.
    float m_timeOfImpact;

    float m_timeLeft;

    /// @protected
    /// @brief Pointer to the first collider in collision.
    PE_Collider *m_colliderA;

    /// @protected
    /// @brief Pointer to the second collider in collision.
    PE_Collider *m_colliderB;

    /// @protected
    /// @brief Collision for the first collider.
    PE_Collision m_collisionA;

    /// @protected
    /// @brief Collision for the second collider.
    PE_Collision m_collisionB;

    /// @protected
    /// @brief A pointer to the solver data.
    PE_SolverCollisionPair *m_solverData;
} PE_CollisionPair;

PE_CollisionPair *PE_CollisionPair_new(PE_Collider *colliderA, PE_Collider *colliderB, PE_Allocator *allocator);
void PE_CollisionPair_free(PE_CollisionPair *collisionPair, PE_Allocator *allocator);

PE_INLINE int PE_CollisionPair_getFlags(PE_CollisionPair *collisionPair)
{
    return collisionPair->m_flags;
}

PE_INLINE Bool PE_CollisionPair_hasOneFlag(PE_CollisionPair *collisionPair, int flags)
{
    return (collisionPair->m_flags & flags);
}

PE_INLINE void PE_CollisionPair_addFlags(PE_CollisionPair *collisionPair, int flags)
{
    collisionPair->m_flags |= flags;
}

PE_INLINE void PE_CollisionPair_removeFlags(PE_CollisionPair *collisionPair, int flags)
{
    collisionPair->m_flags &= ~flags;
}

PE_INLINE void PE_CollisionPair_setTimeLeft(PE_CollisionPair *collisionPair, float timeLeft)
{
    collisionPair->m_timeLeft = timeLeft;
}

PE_INLINE float PE_CollisionPair_getTimeLeft(PE_CollisionPair *collisionPair)
{
    return collisionPair->m_timeLeft;
}

PE_INLINE float PE_CollisionPair_addToTimeLeft(PE_CollisionPair *collisionPair, float delta)
{
    return collisionPair->m_timeLeft += delta;
}

PE_INLINE PE_SolverCollisionPair *PE_CollisionPair_getSolverData(PE_CollisionPair *collisionPair)
{
    return collisionPair->m_solverData;
}

PE_INLINE PE_SolverCollisionPair *PE_Collision_getSolverData(PE_Collision *collision)
{
    return PE_CollisionPair_getSolverData(collision->m_collisionPair);
}

void PE_CollisionPair_update(PE_CollisionPair *collisionPair);

// COLLISION FLAGS

PE_INLINE int PE_Collision_getFlags(PE_Collision *collision)
{
    return PE_CollisionPair_getFlags(collision->m_collisionPair);
}

PE_INLINE Bool PE_Collision_hasOneFlag(PE_Collision *collision, int flags)
{
    return PE_CollisionPair_hasOneFlag(collision->m_collisionPair, flags);
}

PE_INLINE void PE_Collision_addFlags(PE_Collision *collision, int flags)
{
    PE_CollisionPair_addFlags(collision->m_collisionPair, flags);
}

PE_INLINE void PE_Collision_removeFlags(PE_Collision *collision, int flags)
{
    PE_CollisionPair_removeFlags(collision->m_collisionPair, flags);
}

// COLLISION USER METHODS

/// @ingroup PE_Collision
/// @brief Indique si une collision implique deux colliders qui se touchent.
/// @param[in] collision la collision.
/// @return TRUE si la collision concerne deux colliders qui se touchent,
/// FALSE sinon.
PE_API PE_INLINE Bool PE_Collision_isTouching(PE_Collision *collision)
{
    return PE_CollisionPair_hasOneFlag(collision->m_collisionPair, PE_COLLISION_TOUCHING);
}

/// @ingroup PE_Collision
/// @brief Renvoie le collider principal impliqué dans une collision.
/// @param[in] collision la collision.
/// @return Le collider principal impliqué dans la collision.
PE_API PE_INLINE PE_Collider *PE_Collision_getCollider(PE_Collision *collision)
{
    return collision->m_collider;
}

/// @ingroup PE_Collision
/// @brief Renvoie le second collider impliqué dans une collision.
/// @param[in] collision la collision.
/// @return Le second collider impliqué dans la collision.
PE_API PE_INLINE PE_Collider *PE_Collision_getOtherCollider(PE_Collision *collision)
{
    return collision->m_otherCollider;
}

/// @ingroup PE_Collision
/// @brief Renvoie le corps principal impliqué dans une collision.
/// @param[in] collision la collision.
/// @return Le corps principal impliqué dans la collision.
PE_API PE_INLINE PE_Body *PE_Collision_getBody(PE_Collision *collision)
{
    return PE_Collider_getBody(collision->m_collider);
}

/// @ingroup PE_Collision
/// @brief Renvoie le second corps impliqué dans une collision.
/// @param[in] collision la collision.
/// @return Le second corps impliqué dans la collision.
PE_API PE_INLINE PE_Body *PE_Collision_getOtherBody(PE_Collision *collision)
{
    return PE_Collider_getBody(collision->m_otherCollider);
}

/// @ingroup PE_Collision
/// @brief Renvoie la position relative du collider principal par rapport au second collider
/// impliqué dans une collision.
/// @param[in] collision la collision.
/// @return La position relative du collider principal par rapport au second collider.
PE_API PE_INLINE int PE_Collision_getRelativePosition(PE_Collision *collision)
{
    return collision->m_relPos;
}

/// @ingroup PE_Collision
/// @brief Renvoie le manifold d'une collision.
/// @param[in] collision la collision.
/// @return Un pointeur vers le manifold de la collision.
PE_API PE_INLINE PE_Manifold *PE_Collision_getManifold(PE_Collision *collision)
{
    return &collision->m_manifold;
}

#endif
