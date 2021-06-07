
#ifndef _PE_SAP_BOX_H_
#define _PE_SAP_BOX_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"
#include "PE_Collider.h"
#include "PE_Math.h"


typedef struct PE_BoxManager_s PE_BoxManager;
typedef struct PE_SapProjections_s PE_SapProjections;

typedef enum PE_SapBoxFlag_e
{
    PE_SAP_BOX_TO_SYNC = 1 << 0,
} PE_SapBoxFlag;

typedef struct PE_SapBox_s
{
    PE_Collider *m_collider;
    uint32 m_minIndex[2];
    uint32 m_maxIndex[2];
    PE_Node m_toSyncNode;
    uint32 m_flags;
} PE_SapBox;

PE_INLINE PE_Collider *PE_SapBox_getCollider(PE_SapBox *box)
{
    return box->m_collider;
}

PE_INLINE uint32 PE_SapBox_getMinEndPointIndex(PE_SapBox *box, int axisIndex)
{
    return box->m_minIndex[axisIndex];
}

PE_INLINE uint32 PE_SapBox_getMaxEndPointIndex(PE_SapBox *box, int axisIndex)
{
    return box->m_maxIndex[axisIndex];
}

PE_INLINE void PE_SapBox_setMinEndPointIndex(PE_SapBox *box, int axisIndex, uint32 minIndex)
{
    box->m_minIndex[axisIndex] = minIndex;
}

PE_INLINE void PE_SapBox_setMaxEndPointIndex(PE_SapBox *box, int axisIndex, uint32 maxIndex)
{
    box->m_maxIndex[axisIndex] = maxIndex;
}

PE_INLINE void PE_SapBox_addFlags(PE_SapBox *box, uint32 flags)
{
    box->m_flags |= flags;
}

PE_INLINE void PE_SapBox_removeFlags(PE_SapBox *box, uint32 flags)
{
    box->m_flags &= ~flags;
}

PE_INLINE Bool PE_SapBox_hasOneFlag(PE_SapBox *box, uint32 flags)
{
    return (box->m_flags & flags);
}

PE_INLINE Bool PE_SapBox_hasAllFlags(PE_SapBox *box, uint32 flags)
{
    return ((box->m_flags & flags) == flags);
}

PE_INLINE Bool PE_SapBox_Overlap(PE_SapBox *boxA, PE_SapBox *boxB)
{
    return PE_AABB_overlap(
        PE_Collider_getAABB(boxA->m_collider),
        PE_Collider_getAABB(boxB->m_collider));
}

//void PE_SapBox_updateEndPoints(PE_SapBox *box, PE_SapProjections *projections);

void PE_SapBox_updateAxisEndPoints(PE_SapBox *box, PE_SapProjections *projections, int axisIndex);

void PE_SapBox_setToInfinity(PE_SapBox *box, PE_SapProjections *projections);

#endif
