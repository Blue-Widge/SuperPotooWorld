
#ifndef _PE_SAP_INTERVALS_H_
#define _PE_SAP_INTERVALS_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"

typedef struct PE_SapIntervals_s
{
    PE_Allocator *m_allocator;
    uint32 *m_activeBoxIDs;
    uint32 m_nbActiveBoxIDs;
    uint32 m_capacity;
} PE_SapIntervals;

PE_SapIntervals *PE_SapIntervals_new(PE_Allocator *allocator);
void PE_SapIntervals_free(PE_SapIntervals *intervals);

int PE_SapIntervals_attachBox(PE_SapIntervals *intervals, uint32 boxID);

void PE_SapIntervals_removeBox(PE_SapIntervals *intervals, uint32 boxID);

PE_INLINE void PE_SapIntervals_clear(PE_SapIntervals *intervals)
{
    intervals->m_activeBoxIDs = 0;
}

PE_INLINE uint32 *PE_SapIntervals_getActiveBoxIDs(PE_SapIntervals *intervals)
{
    return intervals->m_activeBoxIDs;
}

PE_INLINE uint32 PE_SapIntervals_getNbActiveBoxIDs(PE_SapIntervals *intervals)
{
    return intervals->m_nbActiveBoxIDs;
}

#endif