#include "PVector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int PVector_setCapacity(PVector *vector, int newCapacity)
{
    int capacity = vector->m_capacity;
    void *newData = NULL;

    if (newCapacity <= capacity)
        return EXIT_SUCCESS;

    newData = (void *)calloc(newCapacity, sizeof(void **));
    if (!newData) goto ERROR_LABEL;

    PE_Memcpy(
        newData, (size_t)newCapacity * sizeof(void **),
        vector->m_array, (size_t)capacity * sizeof(void **)
    );

    free(vector->m_array);

    vector->m_array = newData;
    vector->m_capacity = newCapacity;


    //printf("INFO - PVector_setCapacity\n");
    //printf("     - New capacity = %d\n", newCapacity);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - PVector_setCapacity()\n");
    return EXIT_FAILURE;
}

PVector *PVector_new(int capacity)
{
    PVector *vector = NULL;

    if (capacity < 2)
        capacity = 2;

    vector = (PVector *)calloc(1, sizeof(PVector));
    if (!vector) goto ERROR_LABEL;

    vector->m_array = NULL;
    vector->m_capacity = capacity;
    vector->m_size = 0;

    vector->m_array = (void *)calloc(capacity, sizeof(void **));
    if (!vector->m_array) goto ERROR_LABEL;

    return vector;

ERROR_LABEL:
    printf("ERROR - PVector_new()\n");
    PVector_free(vector);
    return NULL;
}

void PVector_free(PVector *vector)
{
    if (!vector)
        return;

    free(vector->m_array);
    free(vector);
}

int PVector_pushBack(PVector *vector, void *data)
{    
    int size = vector->m_size;

    if (size >= vector->m_capacity)
    {
        // Double la capacité
        int exitStatus = PVector_setCapacity(vector, vector->m_capacity << 1);
        if (exitStatus == EXIT_FAILURE) goto ERROR_LABEL;
    }
    vector->m_array[size] = data;
    vector->m_size = size + 1;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - PVector_pushBack()\n");
    return EXIT_FAILURE;
}

int PVector_copy(PVector *destination, void **source, int nbElements)
{
    if (destination->m_capacity < nbElements)
    {
        int exitStatus = PVector_setCapacity(destination, nbElements);
        if (exitStatus != EXIT_SUCCESS)
        {
            printf("ERROR - PVector_copy()\n");
            return EXIT_FAILURE;
        }
    }

    PE_Memcpy(
        destination->m_array, (size_t)nbElements * sizeof(void **),
        source, (size_t)nbElements * sizeof(void **)
    );

    return EXIT_SUCCESS;
}
