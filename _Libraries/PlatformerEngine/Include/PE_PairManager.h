
#ifndef _PE_PAIR_MANAGER_H_
#define _PE_PAIR_MANAGER_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"
#include "PE_Collider.h"
#include "PE_Vector.h"

//-------------------------------------------------------------------------------------------------
// PE_Pair

typedef enum PE_PairFlag_e
{
    PE_PAIR_NEW       = 1 << 0,
    PE_PAIR_TO_REMOVE = 1 << 1,
    PE_PAIR_IN_STEP   = 1 << 2,
} PE_PairFlag;

typedef struct PE_Pair_s
{
    uint32 m_idA;
    uint32 m_idB;
    uint32 m_flags;
} PE_Pair;

PE_INLINE Bool PE_Pair_hasOneFlag(PE_Pair *pair, int flags)
{
    return (pair->m_flags & flags);
}

PE_INLINE void PE_Pair_addFlags(PE_Pair *pair, int flags)
{
    pair->m_flags |= flags;
}

PE_INLINE void PE_Pair_removeFlags(PE_Pair *pair, int flags)
{
    pair->m_flags &= ~flags;
}

PE_INLINE uint32 PE_Pair_getIDA(PE_Pair *pair)
{
    return pair->m_idA;
}

PE_INLINE uint32 PE_Pair_getIDB(PE_Pair *pair)
{
    return pair->m_idB;
}

//-------------------------------------------------------------------------------------------------
// PE_PairContainer

typedef struct PE_PairContainer_s
{
    uint32 m_mask;
    int m_nbPairs;
    int m_hashSize;
    int m_capacity;
    uint32 *m_hashTable;
    uint32 *m_next;
    PE_Pair *m_pairs;

    PE_Allocator *m_allocator;
} PE_PairContainer;

PE_PairContainer *PE_PairContainer_new(PE_Allocator *allocator);
void PE_PairContainer_free(PE_PairContainer *pairContainer);
int PE_PairContainer_doubleCapacity(PE_PairContainer *pairContainer);

uint32 PE_PairContainer_findPair(PE_PairContainer *pairContainer, uint32 idA, uint32 idB);
uint32 PE_PairContainer_createPair(PE_PairContainer *pairContainer, uint32 idA, uint32 idB);
Bool PE_PairContainer_removePair(PE_PairContainer *pairContainer, uint32 idA, uint32 idB);
void PE_PairContainer_clearPairs(PE_PairContainer *pairContainer);

PE_INLINE PE_Pair *PE_PairContainer_getPair(PE_PairContainer *pairContainer, uint32 index)
{
    return &pairContainer->m_pairs[index];
}

PE_INLINE int PE_PairContainer_getNbPairs(PE_PairContainer *pairContainer)
{
    return pairContainer->m_nbPairs;
}

PE_INLINE PE_Pair *PE_PairContainer_getPairs(PE_PairContainer *pairContainer)
{
    return pairContainer->m_pairs;
}

//-------------------------------------------------------------------------------------------------
// PE_PairManager

typedef struct PE_PairManager_s
{
    /// @brief The pair container.
    PE_PairContainer *m_pairContainer;

    /// @brief The vector containing every pair that has either been created or removed
    /// since the last call of PE_PairManager_step().
    PE_Vector *m_stepIndices;
    PE_Vector *m_createdPairs;
    PE_Vector *m_removedPairs;
    PE_Vector *m_toRemove;

    PE_Allocator *m_allocator;
} PE_PairManager;

PE_PairManager *PE_PairManager_new(PE_Allocator *allocator);
void PE_PairManager_free(PE_PairManager *pairManager);

int PE_PairManager_createPair(PE_PairManager *pairManager, uint32 idA, uint32 idB);
int PE_PairManager_removePair(PE_PairManager *pairManager, uint32 idA, uint32 idB);
int PE_PairManager_step(PE_PairManager *pairManager);

PE_INLINE void PE_PairManager_clearPairs(PE_PairManager *pairManager)
{
    PE_PairContainer_clearPairs(pairManager->m_pairContainer);
}

PE_INLINE int PE_PairManager_getNbPairs(PE_PairManager *pairManager)
{
    return PE_PairContainer_getNbPairs(pairManager->m_pairContainer);
}

PE_INLINE int PE_PairManager_getNbCreatedPairs(PE_PairManager *pairManager)
{
    return PE_Vector_getSize(pairManager->m_createdPairs);
}

PE_INLINE int PE_PairManager_getNbRemovedPairs(PE_PairManager *pairManager)
{
    return PE_Vector_getSize(pairManager->m_removedPairs);
}

PE_INLINE int PE_PairManager_getNbStepIndices(PE_PairManager *pairManager)
{
    return PE_Vector_getSize(pairManager->m_stepIndices);
}

PE_INLINE PE_Pair *PE_PairManager_getPairs(PE_PairManager *pairManager)
{
    return PE_PairContainer_getPairs(pairManager->m_pairContainer);
}

PE_INLINE PE_Pair *PE_PairManager_getCreatedPairs(PE_PairManager *pairManager)
{
    return (PE_Pair *)PE_Vector_toArray(pairManager->m_createdPairs);
}

PE_INLINE PE_Pair *PE_PairManager_getRemovedPairs(PE_PairManager *pairManager)
{
    return (PE_Pair *)PE_Vector_toArray(pairManager->m_removedPairs);
}

#endif
