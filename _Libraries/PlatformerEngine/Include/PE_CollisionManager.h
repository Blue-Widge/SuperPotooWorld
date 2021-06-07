
#ifndef _PE_COLLISION_MANAGER_H_
#define _PE_COLLISION_MANAGER_H_

#include "PE_List.h"
#include "PE_Allocator.h"

typedef struct PE_World_s PE_World;
typedef struct PE_CollisionPair_s PE_CollisionPair;
typedef struct PE_TriggerPair_s PE_TriggerPair;
typedef struct PE_Collision_s PE_Collision;
typedef struct PE_Trigger_s PE_Trigger;
typedef struct PE_Collider_s PE_Collider;

typedef struct PE_CollisionManager_s
{
    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief Pointer to the parent world.
    PE_World *m_world;

    /// @protected
    /// @brief The list of every current collision pairs.
    PE_List m_collisionPairList;

    /// @protected
    /// @brief The list of every current trigger pairs.
    PE_List m_triggerPairList;

} PE_CollisionManager;

PE_CollisionManager *PE_CollisionManager_new(PE_Allocator *allocator);
void PE_CollisionManager_free(PE_CollisionManager *collisionManager);

/// @brief 
/// @param detector this.
/// @param colliderA the first collider.
/// @param colliderB the second collider.
/// @return A pointer to the collision pair, or NULL if the two colliders are not in collision.
PE_CollisionPair *PE_CollisionManager_findCollisionPair(PE_Collider *colliderA, PE_Collider *colliderB);

PE_TriggerPair *PE_CollisionManager_findTriggerPair(PE_Collider *colliderA, PE_Collider *colliderB);

PE_CollisionPair *PE_CollisionManager_createCollisionPair(
    PE_CollisionManager *collisionManager, PE_Collider *colliderA, PE_Collider *colliderB);

PE_TriggerPair *PE_CollisionManager_createTriggerPair(
    PE_CollisionManager *collisionManager, PE_Collider *colliderA, PE_Collider *colliderB);

void PE_CollisionManager_removeCollisionPair(PE_CollisionManager *collisionManager, PE_CollisionPair *collisionPair);
void PE_CollisionManager_removeTriggerPair(PE_CollisionManager *collisionManager, PE_TriggerPair *triggerPair);

PE_INLINE int PE_CollisionManager_getNbCollisions(PE_CollisionManager *collisionManager)
{
    return PE_List_getSize(&collisionManager->m_collisionPairList);
}

PE_INLINE int PE_CollisionManager_getNbTriggers(PE_CollisionManager *collisionManager)
{
    return PE_List_getSize(&collisionManager->m_triggerPairList);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PE_CollisionPairIterator

typedef PE_Iterator PE_CollisionPairIterator;

/// @brief Gets an iterator over the collisions of the collider.
/// @param[in] collider this.
/// @param it the iterator over the collision collection.
///
/// @sa PE_CollisionPairIterator_moveNext(), PE_CollisionPairIterator_current(), PE_CollisionPairIterator_reset()
PE_INLINE void PE_CollisionManager_getCollisionPairIterator(
    PE_CollisionManager *collisionManager, PE_CollisionPairIterator *it)
{
    PE_List_getIterator(&collisionManager->m_collisionPairList, it);
}

/// @brief Advances the iterator to the next element of the collection.
/// @param[in,out] it this.
/// @return FALSE if the iterator has passed the end of the collection, TRUE otherwise.
PE_INLINE Bool PE_CollisionPairIterator_moveNext(PE_CollisionPairIterator *it)
{
    return PE_Iterator_moveNext(it);
}

/// @brief Gets a pointer to the element in the collection at the current position of the iterator.
/// @param[in] it this.
/// @return The element in the collection at the current position of the iterator.
PE_INLINE PE_CollisionPair *PE_CollisionPairIterator_current(PE_CollisionPairIterator *it)
{
    return (PE_CollisionPair *)PE_Iterator_current(it);
}

/// @brief Sets the iterator to its initial position,
/// which is before the first element in the collection.
/// @param[in,out] it this.
PE_INLINE void PE_CollisionPairIterator_reset(PE_CollisionPairIterator *it)
{
    PE_Iterator_reset(it);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PE_TriggerPairIterator

typedef PE_Iterator PE_TriggerPairIterator;

/// @brief Gets an iterator over the collisions of the collider.
/// @param[in] collider this.
/// @param it the iterator over the collision collection.
///
/// @sa PE_TriggerPairIterator_moveNext(), PE_TriggerPairIterator_current(), PE_TriggerPairIterator_reset()
PE_INLINE void PE_CollisionManager_getTriggerPairIterator(
    PE_CollisionManager *collisionManager, PE_TriggerPairIterator *it)
{
    PE_List_getIterator(&collisionManager->m_triggerPairList, it);
}

/// @brief Advances the iterator to the next element of the collection.
/// @param[in,out] it this.
/// @return FALSE if the iterator has passed the end of the collection, TRUE otherwise.
PE_INLINE Bool PE_TriggerPairIterator_moveNext(PE_TriggerPairIterator *it)
{
    return PE_Iterator_moveNext(it);
}

/// @brief Gets a pointer to the element in the collection at the current position of the iterator.
/// @param[in] it this.
/// @return The element in the collection at the current position of the iterator.
PE_INLINE PE_TriggerPair *PE_TriggerPairIterator_current(PE_TriggerPairIterator *it)
{
    return (PE_TriggerPair *)PE_Iterator_current(it);
}

/// @brief Sets the iterator to its initial position,
/// which is before the first element in the collection.
/// @param[in,out] it this.
PE_INLINE void PE_TriggerPairIterator_reset(PE_TriggerPairIterator *it)
{
    PE_Iterator_reset(it);
}

#endif
