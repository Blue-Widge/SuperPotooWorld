
#ifndef _PE_SAP_PROJECTIONS_H_
#define _PE_SAP_PROJECTIONS_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"
#include "PE_BoxManager.h"
#include "PE_Vector.h"

//-------------------------------------------------------------------------------------------------
// PE_SapEndPoint

extern const uint32 PE_MAX_END_POINT;

typedef struct PE_SapEndPoint_s
{
    /// @brief Owner box ID | PE_MAX_END_POINT flag.
    uint32 m_data;

    /// @brief The value of this end point.
    float m_value;
} PE_SapEndPoint;

PE_INLINE float PE_SapEndPoint_getValue(PE_SapEndPoint *endPoint)
{
    return endPoint->m_value;
}

PE_INLINE void PE_SapEndPoint_setValue(PE_SapEndPoint *endPoint, float value)
{
    endPoint->m_value = value;
}

PE_INLINE Bool PE_SapEndPoint_isMax(PE_SapEndPoint *endPoint)
{
    return (endPoint->m_data & PE_MAX_END_POINT);
}

PE_INLINE Bool PE_SapEndPoint_isMin(PE_SapEndPoint *endPoint)
{
    return !(endPoint->m_data & PE_MAX_END_POINT);
}

PE_INLINE uint32 PE_SapEndPoint_getBoxID(PE_SapEndPoint *endPoint)
{
    return (endPoint->m_data & ~PE_MAX_END_POINT);
}

//-------------------------------------------------------------------------------------------------
// PE_SapProjections

typedef struct PE_SapProjections_s
{
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @brief Vectors of end points over X and Y.
    PE_Vector *m_endPoints[2];
} PE_SapProjections;

PE_SapProjections *PE_SapProjections_new(PE_Allocator *allocator);
void PE_SapProjections_free(PE_SapProjections *projections);

int PE_SapProjections_attachBox(PE_SapProjections *projections, PE_BoxManager *boxManager, uint32 boxID);
int PE_SapProjections_removeLastBox(PE_SapProjections *projections);

void PE_SapProjections_swapEndPoints(
    PE_SapProjections *projections, PE_BoxManager *boxManager,
    int axisIndex, int indexA, int indexB);

PE_INLINE PE_SapEndPoint* PE_SapProjections_getEndPoint(PE_SapProjections *projections, int axisIndex, int endPointIndex)
{
    return (PE_SapEndPoint *)PE_Vector_get(projections->m_endPoints[axisIndex], endPointIndex);
}

PE_INLINE int PE_SapProjections_getNbEndPoints(PE_SapProjections *projections, int axisIndex)
{
    return PE_Vector_getSize(projections->m_endPoints[axisIndex]);
}

#endif
