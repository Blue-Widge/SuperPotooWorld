
#ifndef _PE_ISLAND_H_
#define _PE_ISLAND_H_

#include "PE_Settings.h"
#include "PE_Allocator.h"
#include "PE_Vector.h"
#include "PE_Body.h"
#include "PE_Collision.h"

/// @brief An island is group of bodies that can influence the motion of each other in the group through collisions,
/// but will not influence the movement of objects outside the island.
typedef struct PE_Island_s
{
    PE_Solver *m_solver; // contient une stack pour créer les island
    PE_Allocator *m_allocator;
    PE_Vector *m_pBodies;
    PE_Vector *m_pCollisionPairs;
} PE_Island;

PE_Island *PE_Island_new(PE_Solver *m_solver);
void PE_Island_free(PE_Island *island);

void PE_Island_addBody(PE_Island *island, PE_Body *body);
void PE_Island_addCollisionPair(PE_Island *island, PE_CollisionPair *collisionPair);
void PE_Island_clear(PE_Island *island);

int PE_Island_solveFirstTOI(PE_Island *island);

PE_INLINE int PE_Island_getNbBodies(PE_Island *island)
{
    return PE_Vector_getSize(island->m_pBodies);
}

PE_INLINE PE_Body *PE_Island_getBody(PE_Island *island, int index)
{
    return *(PE_Body **)PE_Vector_get(island->m_pBodies, index);
}

PE_INLINE int PE_Island_getNbCollisionPairs(PE_Island *island)
{
    return PE_Vector_getSize(island->m_pCollisionPairs);
}

PE_INLINE PE_CollisionPair *PE_Island_getCollisionPair(PE_Island *island, int index)
{
    return *(PE_CollisionPair **)PE_Vector_get(island->m_pCollisionPairs, index);
}


#endif
