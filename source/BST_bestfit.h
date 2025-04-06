#ifndef  BST_BESTFIT_H_DEFINED
#define  BST_BESTFIT_H_DEFINED

#include <stdbool.h>
#include "Disk.h"

typedef struct AVL_tree_t AVL_tree;

AVL_tree *avl_create();

void avl_free_with_freeDisk(AVL_tree *tree);

void avl_free_without_freeDisk(AVL_tree *tree);

int avl_height(AVL_tree *tree);

bool avl_insert(AVL_tree *tree, Disk *d);

bool avl_delete_with_free(AVL_tree *tree, Disk *d);

bool avl_delete_without_free(AVL_tree *tree, Disk *d);

Disk *avl_find(AVL_tree *tree, Disk *d);

Disk *avl_successor(AVL_tree *tree, Disk *d);

#endif // BST_BESTFIT_H_DEFINED