
#ifndef _PE_SLEEP_H_
#define _PE_SLEEP_H_

#include "PE_Math.h"
#include "PE_Allocator.h"

typedef struct PE_Sleep_s
{
    PE_Vec2 *m_lastPositions;
    int m_index;
    float m_drowsinessTime;
    float m_accumulator;
} PE_Sleep;

PE_Sleep *PE_Sleep_new(PE_Allocator *allocator);
void PE_Sleep_free(PE_Sleep *sleep, PE_Allocator *allocator);

void PE_Sleep_setNewPosition(PE_Sleep *sleep, PE_Vec2 *position, float timeStep);

PE_INLINE Bool PE_Sleep_isTimeToSleep(PE_Sleep *sleep)
{
    return (sleep->m_drowsinessTime > PE_TIME_TO_SLEEP);
}

PE_INLINE void PE_Sleep_wakeUp(PE_Sleep *sleep)
{
    sleep->m_drowsinessTime = 0.0f;
}

#endif