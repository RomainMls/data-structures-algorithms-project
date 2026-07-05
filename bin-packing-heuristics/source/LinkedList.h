#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Represents a node of a linked list.
 */
typedef struct Node_t Node;

/**
 * @brief Represents a linked list.
 */
typedef struct List_t List;

/**
 * @brief Returns the data stored in a node.
 *
 * @param node Node to get the data from.
 *
 * @return The data stored in the node.
 */
void *llData(Node *node);

/**
 * @brief Get the next node of a node.
 *
 * @param node Node to get the next node from.
 *
 * @return The next node.
 */
Node *llNext(Node *node);

/**
 * @brief Creates a new empty list.
 *
 * @return The created list.
 */
List *llCreateEmpty(void);

/**
 * @brief Gets the first node of a list.
 *
 * @param list List to get the first node from.
 *
 * @return the first node of the list.
 */
Node *llHead(const List *list);

/**
 * @brief Gets the last node of a list.
 *
 * @param list List to get the last node from.
 *
 * @return the last node of the list.
 */
Node *llTail(const List *list);

/**
 * @brief Gets the length of a list.
 *
 * @param list List to get the length from.
 *
 * @return the length of the list.
 */
size_t llLength(const List *list);

/**
 * @brief Frees a list, but not the data stored in the nodes.
 *
 * @param list List to free.
 */
void llFree(List *list);

/**
 * @brief Frees a list, and the data stored in the nodes.
 *
 * @param list List to free.
 */
void llFreeData(List *list, void (*freeData)(void *));

/**
 * @brief Adds a node with data at the beginning of a list.
 *
 * @param list List to add the node to.
 * @param node Node to add.
 */
void llInsertFirst(List *list, void *data);

/**
 * @brief Frees the node at the start of a list and returns a pointer
 * to its data.
 *
 * @param list List to remove the node from.
 *
 * @return The data of the removed node.
 */
void *llPopFirst(List *list);

/**
 * @brief Adds a node with data at the end of a list.
 *
 * @param list List to add the node to.
 * @param node Node to add.
 */
void llInsertLast(List *list, void *data);

/**
 * @brief Sorts the list (in place) using the given comparator.
 *
 * @param list List to be sorted
 * @param compare a function comparing the data value of two nodes.
 *                Return an integer greater than, equal to, or less
 *                than 0, respectively if the first argument is greater than,
 *                equal to, or less than the second argument.
 */
void llSort(List *list, int (*compare)(const void *, const void *));

#endif // !_LINKED_LIST_H_
