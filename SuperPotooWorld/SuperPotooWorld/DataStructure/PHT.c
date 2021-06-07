#include "PHT.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------------------------------------------------------------------------------
// Fonctions auxiliaires

/// @brief Fonction de hachage de la table.
/// @param[in] pointer le pointeur à hacher.
/// @return Le haché du pointeur.
static unsigned int hash(void *pointer)
{
    return (unsigned int)(pointer) >> 3;
}

//-------------------------------------------------------------------------------------------------
// Fonctions principales

PHT *PHT_new(int capacity)
{
    PHT *hashTable = NULL;

    hashTable = (PHT *)calloc(1, sizeof(PHT));
    if (!hashTable) goto ERROR_LABEL;

    hashTable->m_capacity = capacity;
    hashTable->m_size = 0;

    hashTable->m_hashTable = (int *)calloc(capacity, sizeof(int));
    if (!hashTable->m_hashTable) goto ERROR_LABEL;

    hashTable->m_next = (int *)calloc(capacity, sizeof(int));
    if (!hashTable->m_next) goto ERROR_LABEL;

    hashTable->m_array = (void **)calloc(capacity, sizeof(void *));
    if (!hashTable->m_array) goto ERROR_LABEL;

    for (int i = 0; i < capacity; i++)
    {
        hashTable->m_hashTable[i] = -1;
        hashTable->m_next[i] = -1;
    }

    return hashTable;

ERROR_LABEL:
    printf("ERROR - PHT_new()\n");
    PHT_free(hashTable);
    return NULL;
}

void PHT_free(PHT *hashTable)
{
    if (!hashTable) return;

    free(hashTable->m_array);
    free(hashTable->m_hashTable);
    free(hashTable->m_next);
    free(hashTable);
}


int PHT_doubleCapacity(PHT *hashTable)
{
    int capacity = hashTable->m_capacity;
    int size = hashTable->m_size;
    int newCapacity = capacity << 1;

    void **newEntries = NULL;
    int   *newHashTable  = NULL;
    int   *newNext       = NULL;

    // Checks the new capacity
    if (size >= newCapacity) goto ERROR_LABEL;

    // Allocates the memory
    newHashTable = (int *)calloc(newCapacity, sizeof(int));
    if (!newHashTable) goto ERROR_LABEL;

    newNext = (int *)calloc(newCapacity, sizeof(int));
    if (!newNext) goto ERROR_LABEL;

    newEntries = (void **)calloc(newCapacity, sizeof(void *));
    if (!newEntries) goto ERROR_LABEL;

    // Copies the entries
    PE_Memcpy(
        newEntries, size * sizeof(void *),
        hashTable->m_array, size * sizeof(void *));

    // Initializes the new arrays
    for (int i = 0; i < newCapacity; ++i)
    {
        newHashTable[i] = -1;
        newNext[i] = -1;
    }

    // Recomputes the hash table
    for (int i = 0; i < size; i++)
    {
        int hashValue = (int)(hash(newEntries[i])) % newCapacity;
        newNext[i] = newHashTable[hashValue];
        newHashTable[hashValue] = i;
    }

    // Deallocates old data
    free(hashTable->m_hashTable);
    free(hashTable->m_next);
    free(hashTable->m_array);

    // Assigns new pointers and new capacity
    hashTable->m_hashTable = newHashTable;
    hashTable->m_next = newNext;
    hashTable->m_array = newEntries;
    hashTable->m_capacity = newCapacity;

    //printf("INFO - PHT_doubleCapacity\n");
    //printf("     - New capacity = %d\n", newCapacity);

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - PHT_doubleCapacity()\n");
    free(newEntries);
    free(newHashTable);
    free(newNext);
    return EXIT_FAILURE;
}

int PHT_findHash(PHT *hashTable, int hashValue, void *pointer)
{
    int capacity = hashTable->m_capacity;
    int index = hashTable->m_hashTable[hashValue];

    void **entries = hashTable->m_array;
    int *next = hashTable->m_next;

    while ((index >= 0) && (index < capacity) && (entries[index] != pointer))
    {
        index = next[index];
    }

    return index;
}

int PHT_find(PHT *hashTable, void *pointer)
{
    int hashValue = (int)(hash(pointer) % hashTable->m_capacity);
    return PHT_findHash(hashTable, hashValue, pointer);
}

int PHT_add(PHT *hashTable, void *pointer)
{
    int hashValue = (int)(hash(pointer) % hashTable->m_capacity);
    int index = PHT_findHash(hashTable, hashValue, pointer);

    // If the key is in the table
    if (index >= 0)
    {
        return EXIT_SUCCESS;
    }

    // Checks if the table is full
    int size = hashTable->m_size;
    if (size + 1 >= hashTable->m_capacity)
    {
        // Increases the capacity of the table
        int exitStatus = PHT_doubleCapacity(hashTable);
        if (exitStatus == EXIT_FAILURE) goto ERROR_LABEL;

        // Recomputes the hash value with new mask
        hashValue = (int)(hash(pointer) % hashTable->m_capacity);
    }

    // Creates a new entry
    hashTable->m_next[size] = hashTable->m_hashTable[hashValue];
    hashTable->m_hashTable[hashValue] = size;
    hashTable->m_size++;

    hashTable->m_array[size] = pointer;

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - PHT_set()\n");
    return EXIT_FAILURE;
}

void PHT_removeFromHashChain(PHT *hashTable, int index, int hashValue)
{
    int *next = hashTable->m_next;
    int prevIndex = -1;
    int currIndex = hashTable->m_hashTable[hashValue];

    // Walks the hash table to find the previous index in the chain
    while (currIndex != index)
    {
        prevIndex = currIndex;
        currIndex = next[currIndex];
    }

    if (prevIndex < 0)
    {
        // The key is the first in its hash chain
        hashTable->m_hashTable[hashValue] = next[index];
    }
    else
    {
        // The key is inside its hash chain
        next[prevIndex] = next[index];
    }
}

int PHT_remove(PHT *hashTable, void *pointer)
{
    // Searches the key in the table
    int capacity = hashTable->m_capacity;
    int hashValue = (int)(hash(pointer) % capacity);
    int index = PHT_findHash(hashTable, hashValue, pointer);

    if (index < 0)
        return EXIT_FAILURE;

    // Removes the key from its hash chain
    PHT_removeFromHashChain(hashTable, index, hashValue);

    // We overwrite its entry with the last entry to fill the hole in the entries array
    int lastIndex = hashTable->m_size - 1;
    if (lastIndex == index)
    {
        // The entry to remove is the last in the entries array
        hashTable->m_size--;
        return EXIT_SUCCESS;
    }

    // Computes the hash value of the last entry
    hashValue = (int)(hash(hashTable->m_array[lastIndex]) % capacity);

    // Removes the last key from its hash chain
    PHT_removeFromHashChain(hashTable, lastIndex, hashValue);

    // Inserts the last entry in the free slot
    hashTable->m_array[index] = hashTable->m_array[lastIndex];
    hashTable->m_next[index] = hashTable->m_hashTable[hashValue];
    hashTable->m_hashTable[hashValue] = index;
    hashTable->m_size--;

    // Clears the last entry
    hashTable->m_array[lastIndex] = NULL;

    return EXIT_SUCCESS;
}

void PHT_clear(PHT *hashTable)
{
    void **entries = hashTable->m_array;
    int capacity = hashTable->m_capacity;
    int size = hashTable->m_size;

    for (int i = 0; i < capacity; ++i)
    {
        hashTable->m_hashTable[i] = -1;
        hashTable->m_next[i] = -1;
    }

    for (int i = 0; i < size; ++i)
    {
        entries[i] = NULL;
    }

    hashTable->m_size = 0;
}

void PHT_print(PHT *hashTable)
{
    void **entries = PHT_toArray(hashTable);
    int nbEntries = PHT_getSize(hashTable);
    int capacity = PHT_getCapacity(hashTable);
    int *next = hashTable->m_next;

    printf("PHT at 0x%p\n", hashTable);
    printf("  PHT (capacity = %d)\n", capacity);
    for (int hashValue = 0; hashValue < capacity; hashValue++)
    {
        printf("    hashValue = %03X\n", hashValue);
        int index = hashTable->m_hashTable[hashValue];
        while (index >= 0)
        {
            printf("    ->  (%3d) \"%p\"\n", index, entries[index]);
            index = next[index];
        }
    }
    printf("  Entries (nbEntries = %d)\n", nbEntries);
    for (int i = 0; i < nbEntries; i++)
    {
        void *entry = entries[i];
        int hashValue = (int)(hash(entry) % hashTable->m_capacity);
        printf("    Index = %3d, data = %p, hash = %03X\n", i, entry, hashValue);
    }
    printf("  Free entries (count = %d)\n", capacity - nbEntries);
    for (int i = nbEntries; i < capacity; i++)
    {
        void *entry = entries[i];
        printf("    Index = %3d, data = %p\n", i, entry);
    }
}
