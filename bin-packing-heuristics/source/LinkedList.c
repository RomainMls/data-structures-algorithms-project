#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "LinkedList.h"

struct Node_t
{
    void *data;
    Node *next;
};

struct List_t
{
    Node *head;
    Node *tail;
    size_t length;
};

static void _mergesort(Node *first, size_t length,
                       int (*compare)(const void *, const void *),
                       Node **new_first, Node **new_last);

static Node *llCreateNode(void *data);

static Node *llCreateNode(void *data)
{
    Node *n = malloc(sizeof(Node));
    if (!n)
    {
        printf("llCreateNode: allocation error.\n");
        exit(1);
    }

    n->data = data;
    n->next = NULL;

    return n;
}

void *llData(Node *node)
{
    return node->data;
}

Node *llNext(Node *node)
{
    return node->next;
}

List *llCreateEmpty(void)
{
    List *list = malloc(sizeof(List));
    if (!list)
        return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;

    return list;
}

Node *llHead(const List *list)
{
    return list->head;
}

Node *llTail(const List *list)
{
    return list->tail;
}

size_t llLength(const List *list)
{
    return list->length;
}

void llFree(List *list)
{
    Node *n = list->head;
    Node *nn = NULL;

    while (n)
    {
        nn = n->next;
        free(n);
        n = nn;
    }

    free(list);
}

void llFreeData(List *list, void (*freeData)(void *))
{
    Node *n = list->head;
    Node *nn = NULL;

    while (n)
    {
        nn = n->next;
        freeData(n->data);
        free(n);
        n = nn;
    }

    free(list);
}

void llInsertFirst(List *list, void *data)
{
    Node *newnode = llCreateNode(data);

    if (!list->head)
        list->tail = newnode;

    newnode->next = list->head;
    list->head = newnode;
    list->length++;
}

void *llPopFirst(List *list)
{
    if (!list->head)
        return NULL;

    Node *node = list->head;

    list->length--;
    list->head = node->next;
    node->next = NULL;

    if (!list->head)
        list->tail = NULL;

    void *data = node->data;
    free(node);
    return data;
}

void llInsertLast(List *list, void *data)
{
    Node *newnode = llCreateNode(data);
    newnode->next = NULL;

    if (list->head)
    {
        list->tail->next = newnode;
    }
    else
    {
        list->tail = newnode;
        list->head = newnode;
    }
    list->tail = newnode;
    list->length++;
}

static void _mergesort(Node *first, size_t length,
                       int (*compare)(const void *, const void *),
                       Node **new_first, Node **new_last)
{
    if (length == 1)
    {
        *new_first = first;
        *new_last = first;
        return;
    }

    Node *mid = first;
    Node *prev = NULL;
    for (size_t i = 0; i < length / 2; i++)
    {
        prev = mid;
        mid = mid->next;
    }

    prev->next = NULL;

    Node *left_first, *left_last;
    Node *right_first, *right_last;

    _mergesort(first, length / 2, compare, &left_first, &left_last);
    _mergesort(mid, length - length / 2, compare,
               &right_first, &right_last);

    Node *new_head = NULL;
    Node *new_tail = NULL;

    while (left_first && right_first)
    {
        Node *node = NULL;

        if (compare(llData(left_first), llData(right_first)) <= 0)
        {
            node = left_first;
            left_first = left_first->next;
        }
        else
        {
            node = right_first;
            right_first = right_first->next;
        }

        if (!new_head)
            new_head = node;
        else
            new_tail->next = node;

        new_tail = node;
    }

    if (left_first)
    {
        if (!new_head)
            new_head = left_first;
        else
            new_tail->next = left_first;

        new_tail = left_last;
    }
    else if (right_first)
    {
        if (!new_head)
            new_head = right_first;
        else
            new_tail->next = right_first;

        new_tail = right_last;
    }

    *new_first = new_head;
    *new_last = new_tail;
}

void llSort(List *list, int (*compare)(const void *, const void *))
{
    Node *new_head, *new_tail;
    _mergesort(list->head, list->length, compare,
               &new_head, &new_tail);

    list->head = new_head;
    list->tail = new_tail;
}
