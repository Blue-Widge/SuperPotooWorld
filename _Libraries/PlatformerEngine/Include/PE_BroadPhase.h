
#ifndef _PE_BROAD_PHASE_H_
#define _PE_BROAD_PHASE_H_

#include "PE_Allocator.h"
#include "PE_Vector.h"
#include "PE_List.h"
#include "PE_Collider.h"
#include "PE_PairManager.h"
#include "PE_SapProjections.h"
#include "PE_SapIntervals.h"

#include <stdio.h>

typedef struct PE_BroadPhase_s
{
    /// @protected
    /// @brief The list of every SAP box that moved since the last call to PE_BroadPhase_updatePairs()
    PE_Vector *m_toSync;

    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    PE_PairManager *m_pairManager;

    PE_BoxManager *m_boxManager;

    PE_SapProjections *m_projections;

    //PE_SapIntervals *m_intervals;
} PE_BroadPhase;

PE_BroadPhase *PE_BroadPhase_new(PE_Allocator *allocator);
void PE_BroadPhase_free(PE_BroadPhase *bp);

int PE_BroadPhase_attachCollider(PE_BroadPhase *bp, PE_Collider *collider);
void PE_BroadPhase_removeCollider(PE_BroadPhase *bp, PE_Collider *collider);
int PE_BroadPhase_synchronizeCollider(PE_BroadPhase *bp, PE_Collider *collider);

int PE_BroadPhase_updatePairsPersistent(PE_BroadPhase *bp);
//void PE_BroadPhase_updatePairsBruteForce(PE_BroadPhase *bp);

PE_INLINE PE_PairManager *PE_BroadPhase_getPairManager(PE_BroadPhase *bp)
{
    return bp->m_pairManager;
}

PE_INLINE PE_Allocator *PE_BroadPhase_getAllocator(PE_BroadPhase *bp)
{
    return bp->m_allocator;
}

void PE_BroadPhase_removeBox(PE_BroadPhase *bp, PE_SapBox *box);

#endif
