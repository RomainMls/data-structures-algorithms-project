#ifndef  BST_FIRSTFIT_H_DEFINED
#define  BST_FIRSTFIT_H_DEFINED

#include <stdbool.h>
#include "Disk.h"

typedef struct AVL_Node_t AVL_Node;

Disk *getDisk(AVL_Node *n);

typedef struct AVL_tree_t AVL_tree;

AVL_tree *avl_create(void);

void avl_free_without_freeDisk(AVL_tree *tree);

void avl_free_with_freeDisk(AVL_tree *tree);

int avl_height(AVL_tree *tree);

bool avl_insert(AVL_tree *tree, Disk *disk);

AVL_Node *tree_search_ff(AVL_tree *tree, size_t size);

void avl_notify_update(AVL_tree *tree, AVL_Node *modified_node, size_t prev_size);

bool detect_imbalance(AVL_tree *tree);

#endif // BST_FIRSTFIT_H_DEFINED
