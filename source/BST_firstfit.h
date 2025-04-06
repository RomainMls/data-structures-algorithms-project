#ifndef  BST_FIRSTFIT_H_DEFINED
#define  BST_FIRSTFIT_H_DEFINED

#include <stdbool.h>
#include "Disk.h"

typedef struct AVL_tree_t AVL_tree;

AVL_tree *avl_create();

void avl_free(AVL_tree *tree);

int avl_height(AVL_tree *tree);

bool avl_insert(AVL_tree *tree, Disk *disk);

Disk *tree_search_ff(AVL_tree *tree, size_t size);

#endif // BST_BESTFIT_H_DEFINED