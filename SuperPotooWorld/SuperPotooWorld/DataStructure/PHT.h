#ifndef _PHT_H_
#define _PHT_H_

/// @file PHT.h
/// @defgroup PointerHashTable
/// @{

#include "../Settings.h"

/// @brief Structure repr�sentant une table de hachage contenant uniquement des pointeurs.
/// Les �l�ments pr�sents dans la table sont de type (void *).
/// Il est � la charge de l'utilisateur de s'occuper de la m�moire associ�e � ses pointeurs.
/// La cr�ation, la suppression et la recherche d'un �l�ment sont en temps constant
/// quand la taille de la table est inf�rieure � sa capacit� maximale.
/// Si la taille devient supp�rieure � la capacit�, la table de hachage se redimensionne automatiquement
/// mais cette op�ration � une complexit� proportionnelle � la taille de la table.
/// Les �l�ments sont toujours stock�s de fa�on contig�e en m�moire.
typedef struct PHT_s
{
    /// @protected
    /// @brief Le nombre d'�l�ments dans la table de hachage.
    int m_size;

    /// @protected
    /// @brief La capacit� de la table. Il s'agit du nombre maximal d'�l�ment avant un redimensionnement.
    int m_capacity;

    /// @brief La table de hachage
    /// le membre m_hashTable[hashValue] donne l'indice du premier �l�ment dans le tableau m_entries
    /// dans la cha�ne associ�e � hashValue.
    int *m_hashTable;

    /// @brief Tableau repr�sentant les cha�nes de hachage.
    /// Le membre m_next[index] donne l'indice de l'�l�ment suivant dans le tableau m_entries
    /// dans la cha�ne associ�e � hashValue.
    int *m_next;

    /// @brief Tableau des �l�ments de la table de hachage.
    /// Les �l�ments sont toujours stock�s de mani�re contig�e en m�moire
    /// et indic�s de 0 � (m_size - 1).
    void **m_array;
} PHT;

/// @brief Cr�e une nouvelle table de hachage.
/// @param[in] capacity la capacit� initiale de la table (nombre maximal d'�l�ments avant un redimensionnement).
/// @return La table cr��e ou NULL en cas d'erreur.
PHT *PHT_new(int capacity);

/// @brief D�truit une table de hachage pr�alablement cr��e avec PHT_new().
/// @param[in,out] hashTable la table de hachage � d�truire.
void PHT_free(PHT *hashTable);

/// @brief Recherche l'�l�ment associ� � un cl� donn�e dans la table de hachage.
/// @param[in] hashTable la table de hachage.
/// @param[in] pointer la cl� recherch�e.
/// @return L'indice de l'�l�ment ou -1 si la cl� n'existe pas dans la table.
/// L'indice correspond � la position de l'�l�ment dans le tableau PHT_getEntries().
int PHT_find(PHT *hashTable, void *pointer);

/// @brief D�finit la donn�e associ�e � une cl� dans une table de hachage.
/// Si la cl� n'existe pas la table, cette fonction cr�e automatiquement une nouvelle entr�e.
/// @param[in,out] hashTable la table de hachage.
/// @param[in] pointer la donn�e � d�finir.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
int PHT_add(PHT *hashTable, void *pointer);

/// @brief Supprime l'entr�e associ�e � une cl� dans une table de hachage.
/// Cette fonction supprime simplement le pointeur vers la donn�e.
/// @param[in,out] hashTable la table de hachage.
/// @param[in] pointer la cl�.
/// @return EXIT_SUCCESS ou EXIT_FAILURE.
int PHT_remove(PHT *hashTable, void *pointer);

/// @brief Supprime les entr�es d'une table de hachage.
/// Cette fonction supprime simplement les pointeurs vers les donn�es.
/// @param[in,out] hashTable la table de hachage.
void PHT_clear(PHT *hashTable);

/// @brief Renvoie le tableau contenant les entr�es de la table de hachage.
/// Attention, ce tableau n'est accessible qu'en lecture et ne doit pas �tre modifi�.
/// La taille de ce tableau est donn�e par PHT_getNbEntries().
/// @param[in] hashTable la table de hachage.
/// @return Le tableau contenant les entr�es de la table de hachage.
INLINE void **PHT_toArray(PHT *hashTable)
{
    return hashTable->m_array;
}

/// @brief Renvoie le nombre d'�l�ments d'une table de hachage.
/// Il s'agit de la taille du table renvoy� par PHT_getEntries().
/// @param[in] hashTable la table de hachage.
/// @return Le nombre d'�l�ments de la table de hachage.
INLINE int PHT_getSize(PHT *hashTable)
{
    return hashTable->m_size;
}

/// @brief Renvoie la capacit� (nombre maximal d'�l�ments avant un redimensionnement) d'une table de hachage.
/// @param[in] hashTable la table de hachage.
/// @return La capacit� de la table de hachage.
INLINE int PHT_getCapacity(PHT *hashTable)
{
    return hashTable->m_capacity;
}

/// @brief Affiche sur la sortie standard une table de hachage pour du debug.
/// @param[in] hashTable la table de hachage.
void PHT_print(PHT *hashTable);

/// @}

#endif