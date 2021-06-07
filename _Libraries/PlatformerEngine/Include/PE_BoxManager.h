
#ifndef _PE_SAP_BOX_MANAGER_H_
#define _PE_SAP_BOX_MANAGER_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"
#include "PE_SapBox.h"
#include "PE_Vector.h"

/// @brief The sweep and prune (SAP) box manager.
/// - Adding a new box is O(1).
/// - Removing a box is O(1).
/// - The capacity of this container is dynamic.
/// - Each box is associated with and ID (uint32).
///   This ID is smaller in memory than a pointer.
///   It never changes even if the capacity of the box manager changes.
/// - The boxes are not necessarily (but almost) contiguous in memory.
typedef struct PE_BoxManager_s
{
    PE_Allocator *m_allocator;
    PE_Vector *m_boxes;
    PE_Vector *m_freeIndices;
} PE_BoxManager;

PE_BoxManager *PE_BoxManager_new(PE_Allocator *allocator);
void PE_BoxManager_free(PE_BoxManager *boxManager);

uint32 PE_BoxManager_createBox(PE_BoxManager *boxManager);

PE_INLINE PE_SapBox *PE_BoxManager_getBox(PE_BoxManager *boxManager, uint32 boxID)
{
    return (PE_SapBox *)PE_Vector_get(boxManager->m_boxes, boxID);
}

PE_INLINE void PE_BoxManager_removeBox(PE_BoxManager *boxManager, uint32 boxID)
{
    PE_Vector_pushBack(boxManager->m_freeIndices, &boxID);
}

PE_INLINE uint32 PE_BoxManager_getID(PE_BoxManager *boxManager, PE_SapBox *box)
{
    return (uint32)(box - (PE_SapBox *)PE_Vector_toArray(boxManager->m_boxes));
}

PE_INLINE PE_Collider *PE_BoxManager_getCollider(PE_BoxManager *boxManager, uint32 boxID)
{
    return PE_SapBox_getCollider(PE_BoxManager_getBox(boxManager, boxID));
}

#endif
