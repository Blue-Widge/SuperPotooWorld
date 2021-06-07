#ifndef _PHT_H_
#define _PHT_H_

/// @file PHT.h
/// @defgroup PointerHashTable
/// @{

#include "../Settings.h"

/// @brief Structure représentant une table de hachage contenant uniquement des pointeurs.
/// Les éléments présents dans la table sont de type (void *).
/// Il est à la charge de l'utilisateur de s'occuper de la mémoire associée à ses pointeurs.
/// La création, la suppression et la recherche d'un élément sont en temps constant
/// quand la taille de la table est inférieure à sa capacité maximale.
/// Si la taille devient suppérieure à la capacité, la table de hachage se redimensionne automatiquement
/// mais cette opération à une complexité proportionnelle à la taille de la table.
/// Les éléments sont toujours stockés de façon contigüe en mémoire.
typedef struct PHT_s
{
    /// @protected
    /// @brief Le nombre d'éléments dans la table de hachage.
    int m_size;

    /// @protected
    /// @brief La capacité de la table. Il s'agit du nombre maximal d'élément avant un redimensionnement.
    int m_capacity;

    /// @brief La table de hachage
    /// le membre m_hashTable[hashValue] donne l'indice du premier élément dans le tableau m_entries
    /// dans la chaîne associée à hashValue.
    int *m_hashTable;

    /// @brief Tableau représentant les chaînes de hachage.
    /// Le membre m_next[index] donne l'indice de l'élément suivant dans le tableau m_entries
    /// dans la chaîne associée à hashValue.
    int *m_next;

    /// @brief Tableau des éléments de la table de hachage.
    /// Les éléments sont toujours stockés de manière contigüe en mémoire
    /// et indicés de 0 à (m_size - 1).
    void **m_array;
} PHT;

/// @brief Crée une nouvelle table de hachage.
/// @param[in] capacity la capacité initiale de la table (nombre maximal d'éléments avant un redimensionnement).
/// @return La table créée ou NULL en cas d'erreur.
PHT *PHT_new(int capacity);

/// @brief Détruit une table de hachage préalablement créée avec PHT_new().
/// @param[in,out] hashTable la table de hachage à détruire.
void PHT_free(PHT *hashTable);

/// @brief Recherche l'élément associé à un clé donnée dans la table de hachage.
/// @param[in] hashTable la table de hachage.
/// @param[in] pointer la clé recherchée.
/// @return L'indice de l'élément ou -1 si la clé n'existe pas dans la table.
/// L'indice correspond à la position de l'élément dans le tableau PHT_getEntries().
int PHT_find(PHT *hashTable, void *pointer);

/// @brief Définit la donnée associée à une clé dans une table de hachage.
/// Si la clé n'existe pas la table, cette fonction crée automatiquement une nouvelle entrée.
/// @param[in,out] hashTable la table de hachage.
/// @param[in] pointer la donnée à définir.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
int PHT_add(PHT *hashTable, void *pointer);

/// @brief Supprime l'entrée associée à une clé dans une table de hachage.
/// Cette fonction supprime simplement le pointeur vers la donnée.
/// @param[in,out] hashTable la table de hachage.
/// @param[in] pointer la clé.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
int PHT_remove(PHT *hashTable, void *pointer);

/// @brief Supprime les entrées d'une table de hachage.
/// Cette fonction supprime simplement les pointeurs vers les données.
/// @param[in,out] hashTable la table de hachage.
void PHT_clear(PHT *hashTable);

/// @brief Renvoie le tableau contenant les entrées de la table de hachage.
/// Attention, ce tableau n'est accessible qu'en lecture et ne doit pas être modifié.
/// La taille de ce tableau est donnée par PHT_getNbEntries().
/// @param[in] hashTable la table de hachage.
/// @return Le tableau contenant les entrées de la table de hachage.
INLINE void **PHT_toArray(PHT *hashTable)
{
    return hashTable->m_array;
}

/// @brief Renvoie le nombre d'éléments d'une table de hachage.
/// Il s'agit de la taille du table renvoyé par PHT_getEntries().
/// @param[in] hashTable la table de hachage.
/// @return Le nombre d'éléments de la table de hachage.
INLINE int PHT_getSize(PHT *hashTable)
{
    return hashTable->m_size;
}

/// @brief Renvoie la capacité (nombre maximal d'éléments avant un redimensionnement) d'une table de hachage.
/// @param[in] hashTable la table de hachage.
/// @return La capacité de la table de hachage.
INLINE int PHT_getCapacity(PHT *hashTable)
{
    return hashTable->m_capacity;
}

/// @brief Affiche sur la sortie standard une table de hachage pour du debug.
/// @param[in] hashTable la table de hachage.
void PHT_print(PHT *hashTable);

/// @}

#endif