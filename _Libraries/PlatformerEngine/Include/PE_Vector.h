
#ifndef _PE_VECTOR_H_
#define _PE_VECTOR_H_

#include "PE_Allocator.h"

/// @brief An array with dynamic size.
typedef struct PE_Vector_s
{
    /// @protected
    /// @brief Pointer to the world allocator.
    PE_Allocator *m_allocator;

    /// @protected
    /// @brief The array of data.
    void *m_data;

    /// @protected
    /// @brief The maximum number of elements.
    int m_capacity;

    /// @protected
    /// @brief The number of elements.
    int m_size;

    /// @protected
    /// @brief The size of each element.
    int m_dataSize;
} PE_Vector;

/// @brief Constructs a vector (dynamic array). 
/// @param[in] dataSize the number of byte for each object in the pool list.
/// @param[in] capacity the maximum number of objects in the list.
/// @return The vector.
PE_Vector *PE_Vector_new(int dataSize, int capacity, PE_Allocator *allocator);

/// @brief Destructs the vector.
/// Frees all objects.
/// If at least one element contains one external dynamic allocation,
/// you must free the memory before calling this function.
/// You must not dereference or free the list pointer after this call.
/// You should manually set the list pointer to NULL.
/// @param[in,out] vector this.
void PE_Vector_free(PE_Vector *vector);

/// @brief Appends one element to the vector container.
/// The data is copied in this new element.
/// @param[in,out] vector this.
/// @param[in] data the data of the new element.
/// @return EXIT_SUCCESS or EXIT_FAILURE.
int PE_Vector_pushBack(PE_Vector *vector, void *data);

/// @brief Pops the last element of the vector.
/// @param[in,out] vector this.
PE_INLINE void PE_Vector_popBack(PE_Vector *vector)
{
    if (vector->m_size > 0)
        vector->m_size--;
}

void PE_Vector_pop(PE_Vector *vector, int index);

/// @brief Returns the element at the specified position in this vector.
/// @param[in] vector this.
/// @param[in] index the index of the element to return.
/// @return The element at the specified position.
PE_INLINE void *PE_Vector_get(PE_Vector *vector, int index)
{
    return (void *)((char *)vector->m_data + (size_t)index * vector->m_dataSize);
}

PE_INLINE void PE_Vector_getCopy(PE_Vector *vector, int index, void *dst)
{
    void *pointer = PE_Vector_get(vector, index);
    int dataSize = vector->m_dataSize;
    PE_Memcpy(dst, dataSize, pointer, dataSize);
}

/// @brief Sets the element at the specified position in this vector.
/// @param[in,out] vector this.
/// @param[in] index the index of the element
/// @param[in] data the data of the element.
void PE_Vector_set(PE_Vector *vector, int index, void *data);

/// @brief Clears the vector.
/// @param[in,out] vector this.
PE_INLINE void PE_Vector_clear(PE_Vector *vector)
{
    vector->m_size = 0;
}

/// @brief Returns pointer to the underlying array serving as element storage.
/// @param[in] vector this.
/// @return A pointer to the elements in the vector.
PE_INLINE void *PE_Vector_toArray(PE_Vector *vector)
{
    return vector->m_data;
}

/// @brief Returns the number of elements in the vector container.
/// @param[in] vector this.
/// @return The number of elements.
PE_INLINE int PE_Vector_getSize(PE_Vector *vector)
{
    return vector->m_size;
}

/// @brief Returns the maximum number of elements in the vector container.
/// @param[in] vector this.
/// @return The maximum number of elements.
PE_INLINE int PE_Vector_getCapacity(PE_Vector *vector)
{
    return vector->m_capacity;
}

#endif
