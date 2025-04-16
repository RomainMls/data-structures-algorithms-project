#ifndef  BST_BESTFIT_H_DEFINED
#define  BST_BESTFIT_H_DEFINED

#include <stdbool.h>
#include "Disk.h"

typedef struct AVL_Node_t AVL_Node;

Disk *getDisk(AVL_Node *n);

typedef struct AVL_tree_t AVL_tree;

AVL_tree *avl_create(void);

void avl_free(AVL_tree *tree);

AVL_Node *avl_insert(AVL_tree *tree, Disk *d);

void avl_delete(AVL_tree *tree, AVL_Node *n);

AVL_Node *tree_search_bf(AVL_tree *tree, size_t size);

bool detect_imbalance(AVL_tree *tree);

#endif // BST_BESTFIT_H_DEFINED
