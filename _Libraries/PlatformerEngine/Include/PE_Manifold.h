
#ifndef _PE_MANIFOLD_H_
#define _PE_MANIFOLD_H_

#include "PE_Settings.h"
#include "PE_Shape.h"
#include "PE_Math.h"

/// @brief Structure stockant un contact entre deux formes.
typedef struct PE_Manifold_s
{
    PE_Vec2 m_normal;
    float m_penetration;
} PE_Manifold;

void PE_Manifold_collide(
    PE_Manifold *manifold,
    PE_Shape *shapeA, PE_Sweep *sweepA,
    PE_Shape *shapeB, PE_Sweep *sweepB, float alpha);

PE_INLINE void PE_Manifold_reverse(PE_Manifold *manifold)
{
    manifold->m_normal.x = -manifold->m_normal.x;
    manifold->m_normal.y = -manifold->m_normal.y;
}

PE_INLINE void PE_Manifold_copy(PE_Manifold *destination, PE_Manifold *source)
{
    PE_Memcpy(destination, sizeof(PE_Manifold), source, sizeof(PE_Manifold));
}

#endif
