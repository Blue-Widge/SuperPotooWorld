#ifndef _PVECTOR_H_
#define _PVECTOR_H_

#include "../Settings.h"

/// @brief An array with dynamic size.
typedef struct PVector_s
{
    /// @protected
    /// @brief The array of data.
    void **m_array;

    /// @protected
    /// @brief The maximum number of elements.
    int m_capacity;

    /// @protected
    /// @brief The number of elements.
    int m_size;
} PVector;

PVector *PVector_new(int capacity);
void PVector_free(PVector *vector);
int PVector_pushBack(PVector *vector, void *data);

INLINE void *PVector_get(PVector *vector, int index)
{
    return vector->m_array[index];
}

INLINE void PVector_set(PVector *vector, int index, void *data)
{
    if (index < vector->m_size)
        vector->m_array[index] = data;
}

INLINE void PVector_clear(PVector *vector)
{
    vector->m_size = 0;
}

int PVector_copy(PVector *destination, void **source, int nbElements);

INLINE void **PVector_toArray(PVector *vector)
{
    return vector->m_array;
}

INLINE int PVector_getSize(PVector *vector)
{
    return vector->m_size;
}

INLINE int PVector_getCapacity(PVector *vector)
{
    return vector->m_capacity;
}

#endif