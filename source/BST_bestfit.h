#ifndef  BST_BESTFIT_H_DEFINED
#define  BST_BESTFIT_H_DEFINED

#include <stdbool.h>

typedef struct AVL_tree_t AVL_tree;

AVL_tree *create_avl(int (*compare)(const void *, const void *), void (*freeKey)(void *));

void free_avl(AVL_tree *tree);

int avl_height(AVL_tree *tree);

bool avl_insert(AVL_tree *tree, void *key);

bool avl_delete(AVL_tree *tree, void *key);

void *avl_find(AVL_tree *tree, void *key);

#endif // BST_BESTFIT_H_DEFINED