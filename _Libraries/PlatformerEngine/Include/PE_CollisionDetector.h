
#ifndef _PE_COLLISION_DETECTOR_H_
#define _PE_COLLISION_DETECTOR_H_

#include "PE_Allocator.h"
#include "PE_List.h"
#include "PE_Vector.h"
#include "PE_CollisionManager.h"

typedef struct PE_World_s PE_World;
typedef struct PE_BroadPhase_s PE_BroadPhase;
typedef struct PE_CollisionPair_s PE_CollisionPair;
typedef struct PE_TriggerPair_s PE_TriggerPair;
typedef struct PE_Collision_s PE_Collision;
typedef struct PE_Trigger_s PE_Trigger;
typedef struct PE_Collider_s PE_Collider;

typedef void PE_CollisionCallback(PE_Collision *);
typedef void PE_TriggerCallback(PE_Trigger *);

typedef struct PE_CollisionDetector_s
{
    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief Pointer to the parent world.
    PE_World *m_world;

    /// @protected
    /// @brief The broadPhase of the collision detection algorithm.
    PE_BroadPhase *m_broadPhase;

    PE_CollisionManager *m_collisionManager;

    PE_Vector *m_pCollToUpdate;
    PE_Vector *m_pTrigToUpdate;

    PE_CollisionCallback *m_onCollisionEnter;
    PE_CollisionCallback *m_onCollisionStay;
    PE_CollisionCallback *m_onCollisionExit;

    PE_TriggerCallback *m_onTriggerEnter;
    PE_TriggerCallback *m_onTriggerStay;
    PE_TriggerCallback *m_onTriggerExit;

} PE_CollisionDetector;

PE_CollisionDetector *PE_CollisionDetector_new(PE_World *world);
void PE_CollisionDetector_free(PE_CollisionDetector *detector);

//.................................................................................................
// Collider methods

int PE_CollisionDetector_attachCollider(PE_CollisionDetector *detector, PE_Collider *collider);
void PE_CollisionDetector_removeCollider(PE_CollisionDetector *detector, PE_Collider *collider);
int PE_CollisionDetector_synchronizeCollider(PE_CollisionDetector *detector, PE_Collider *collider);

//.................................................................................................
// Fixed update methods

int PE_CollisionDetector_updateCollisions(PE_CollisionDetector *detector);
void PE_CollisionDetector_step(PE_CollisionDetector *detector, float timeStep);

//.................................................................................................
// Solver Methods

/// @brief Validates a collision pair and makes the approriate callbacks.
/// This method is called by the solver.
/// @param detector this.
/// @param collisionPair the collision pair to validate.
void PE_CollisionDetector_validateCollisionPair(PE_CollisionPair *collisionPair);

PE_INLINE void PE_CollisionDetector_getCollisionPairIterator(PE_CollisionDetector *detector, PE_CollisionPairIterator *it)
{
    PE_CollisionManager_getCollisionPairIterator(detector->m_collisionManager, it);
}

// Profiler methods
PE_INLINE int PE_CollisionDetector_getNbCollisions(PE_CollisionDetector *detector)
{
    return PE_CollisionManager_getNbCollisions(detector->m_collisionManager);
}
PE_INLINE int PE_CollisionDetector_getNbTriggers(PE_CollisionDetector *detector)
{
    return PE_CollisionManager_getNbTriggers(detector->m_collisionManager);
}

//.................................................................................................
// Callback setters and getters

PE_INLINE void PE_CollisionDetector_setOnCollisionEnter(
    PE_CollisionDetector *detector,
    PE_CollisionCallback *onCollisionEnter)
{
    detector->m_onCollisionEnter = onCollisionEnter;
}

PE_INLINE void PE_CollisionDetector_setOnCollisionStay(
    PE_CollisionDetector *detector,
    PE_CollisionCallback *onCollisionStay)
{
    detector->m_onCollisionStay = onCollisionStay;
}

PE_INLINE void PE_CollisionDetector_setOnCollisionExit(
    PE_CollisionDetector *detector,
    PE_CollisionCallback *onCollisionExit)
{
    detector->m_onCollisionExit = onCollisionExit;
}

PE_INLINE void PE_CollisionDetector_setOnTriggerEnter(
    PE_CollisionDetector *detector,
    PE_TriggerCallback *onTriggerEnter)
{
    detector->m_onTriggerEnter = onTriggerEnter;
}

PE_INLINE void PE_CollisionDetector_setOnTriggerStay(
    PE_CollisionDetector *detector,
    PE_TriggerCallback *onTriggerStay)
{
    detector->m_onTriggerStay = onTriggerStay;
}

PE_INLINE void PE_CollisionDetector_setOnTriggerExit(
    PE_CollisionDetector *detector,
    PE_TriggerCallback *onTriggerExit)
{
    detector->m_onTriggerExit = onTriggerExit;
}

// Top level callbacks

/// @brief Top level method updating every "onCollisionEnter" callbacks.
/// @param detector this.
/// @param collisionPair the collision pair involved in the callbacks.
void PE_CollisionDetector_onCollisionEnter(PE_CollisionDetector *detector, PE_CollisionPair *collisionPair);

/// @brief Top level method updating every "onCollisionStay" callbacks.
/// @param detector this.
/// @param collisionPair the collision pair involved in the callbacks.
void PE_CollisionDetector_onCollisionStay(PE_CollisionDetector *detector, PE_CollisionPair *collisionPair);

/// @brief Top level method updating every "onCollisionExit" callbacks.
/// @param detector this.
/// @param collisionPair the collision pair involved in the callbacks.
void PE_CollisionDetector_onCollisionExit(PE_CollisionDetector *detector, PE_CollisionPair *collisionPair);

/// @brief Top level method updating every "onTriggerEnter" callbacks.
/// @param detector this.
/// @param triggerPair the trigger pair involved in the callbacks.
void PE_CollisionDetector_onTriggerEnter(PE_CollisionDetector *detector, PE_TriggerPair *triggerPair);

/// @brief Top level method updating every "onTriggerStay" callbacks.
/// @param detector this.
/// @param triggerPair the trigger pair involved in the callbacks.
void PE_CollisionDetector_onTriggerStay(PE_CollisionDetector *detector, PE_TriggerPair *triggerPair);

/// @brief Top level method updating every "onTriggerExit" callbacks.
/// @param detector this.
/// @param triggerPair the trigger pair involved in the callbacks.
void PE_CollisionDetector_onTriggerExit(PE_CollisionDetector *detector, PE_TriggerPair *triggerPair);

#endif
