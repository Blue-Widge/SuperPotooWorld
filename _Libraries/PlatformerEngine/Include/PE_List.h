
#ifndef _PE_LIST_H_
#define _PE_LIST_H_

#include "PE_Settings.h"

typedef struct PE_Node_s PE_Node;

/// @brief A node of a PE_List.
struct PE_Node_s
{
    /// @protected
    /// @brief Pointer to the previous node. Set to the sentinel address if this node is the first.
    PE_Node *m_prev;

    /// @protected
    /// @brief Pointer to the next node. Set to the sentinel address if this node is the last.
    PE_Node *m_next;

    /// @protected
    /// @brief The data of the node.
    void *m_data;
};

PE_INLINE void PE_Node_setData(PE_Node *node, void *data)
{
    node->m_data = data;
}

PE_INLINE void *PE_Node_getData(PE_Node *node)
{
    return node->m_data;
}

/// @brief A doubly linked list.
/// Each method of the list doesn't make any dynamic memory allocation.
/// The list can be enumerated using an iterator.
typedef struct PE_List_s
{
    /// @protected
    /// @brief The sentinel of the list.
    /// This is not a pointer to avoid dynamic memory allocation.
    PE_Node m_sentinel;

    /// @protected
    /// @brief The number of nodes.
    int m_size;
} PE_List;

void PE_List_init(PE_List *list);

PE_INLINE Bool PE_List_isEmpty(PE_List *list)
{
    return (list->m_size == 0);
}

/// @brief Clears the list. Remove every node.
/// @param[in,out] list this.
void PE_List_clear(PE_List *list);

/// @brief Appends a node to the list.
/// @param[in,out] list this.
/// @param[in,out] node the node to append.
void PE_List_pushBack(PE_List *list, PE_Node *node);

/// @brief Remove a node from the list.
/// @param[in,out] list this.
/// @param[in,out] node the node to remove.
void PE_List_pop(PE_List *list, PE_Node *node);

PE_Node *PE_List_getFirst(PE_List *list);

/// @brief Indicates if the list contains the node.
/// @param[in] list this.
/// @param[in] node a node.
/// @return TRUE is the node is in the list, FALSE otherwise.
Bool PE_List_contains(PE_List *list, PE_Node *node);

/// @brief Gets the number of elements in the list.
/// @param[in] list this.
/// @return The number of elements.
PE_INLINE int PE_List_getSize(PE_List *list)
{
    return list->m_size;
}

typedef struct PE_Iterator_s
{
    PE_Node *m_sentinel;
    PE_Node *m_node;
} PE_Iterator;

/// @brief Gets an iterator to enumerate the elements of the list.
/// @param list[in] this.
/// @param it[out] the iterator.
PE_INLINE void PE_List_getIterator(PE_List *list, PE_Iterator *it)
{
    it->m_sentinel = &list->m_sentinel;
    it->m_node = &list->m_sentinel;
}

/// @brief Advances the iterator to the next element of the collection.
/// @param[in,out] it this.
/// @return FALSE if the iterator has passed the end of the collection, TRUE otherwise.
PE_INLINE Bool PE_Iterator_moveNext(PE_Iterator *it)
{
    it->m_node = it->m_node->m_next;
    return (it->m_node) != (it->m_sentinel);
}

/// @brief Gets a pointer to the element in the collection at the current position of the iterator.
/// @param[in] it this.
/// @return The element in the collection at the current position of the iterator.
PE_INLINE void *PE_Iterator_current(PE_Iterator *it)
{
    return it->m_node->m_data;
}

/// @brief Sets the iterator to its initial position,
/// which is before the first element in the collection.
/// @param[in,out] it this.
PE_INLINE void PE_Iterator_reset(PE_Iterator *it)
{
    it->m_node = it->m_sentinel;
}

#endif