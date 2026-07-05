/*
   Copyright (C) 1993 by Matthew Clegg.  All rights reserved.
*/
#include "Disk.h"
#include <stdlib.h>

typedef struct Treap_node_t Treap_node;

typedef struct Treap_tree_t Treap_tree;

void treap_insert
  (Treap_tree *t, Disk *d);
/* Inserts the pair (k,d) into the treap t.  Assumes on entry that no
   node currently exists in the treap with key k. 
*/

void treap_delete(Treap_tree *t, Treap_node *node_to_delete);

Treap_tree *treap_create(void);

void treap_free(Treap_tree *t);

Disk *getDisk(Treap_node *n);

Treap_node *tree_search_bf(Treap_tree *tree, size_t size);