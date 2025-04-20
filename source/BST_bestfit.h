/* treap.h
   Copyright (C) 1993 by Matthew Clegg.  All rights reserved.

   This header file is the interface to an implementation of the
   treap data structure.  Treaps are a randomized data structure for
   solving the dictionary problem.  For more information about
   them, see:

    Aragon, C.R., Seidel, R.G., Randomized search trees,
      in 30th Annual Symposium on Foundations of Computer Science

   In our implementation, every element of the treap consists of
   a key and data.  The keys are used for organizing the treap,
   so that the pair (key, data) with smallest key is stored in
   the leftmost node of the treap.

   Our implementation supports the operations:
     Lookup       -- searches the treap for a matching key 
     Insert       -- inserts a new (key, data) pair into the treap
     Delete       -- deletes a pair (key, data) based on the key
     Lookup_First -- returns the leftmost (key, data) pair
     Delete_First -- removes the leftmost (key, data) pair
     Iterate      -- applies a specifed iterator function to each pair

   The Treap data type is a pointer to a cell which contains the
   address of the root of the treap.  The reason for the use of
   this double indirection is that the root of the treap may change
   over time.  An empty treap can be initialized with the following
   statements:

     Treap_node *root = NULL;
     Treap      T     = &root;

   Thereafter, insert, delete, lookup, etc. operations may be 
   performed on the treap T.
*/
#include "Disk.h"
#include <stdlib.h>

typedef struct Treap_node_t Treap_node;

typedef struct Treap_tree_t Treap_tree;

typedef Disk *treap_data_element;
  /* This data type is filled in by the user. */

typedef Disk *treap_key_type;
  /* The search key type, also filled in by the user. */

typedef int (*treap_comparator) (treap_key_type a, treap_key_type b);
  /* This routine is also supplied by the user.  
     It should return -1, 0, or 1 as to whether a < b, a == b, or a > b.
  */

typedef int (*treap_iterator) (treap_key_type k, treap_data_element d);

void treap_insert
  (Treap_tree *t, treap_data_element d);
/* Inserts the pair (k,d) into the treap t.  Assumes on entry that no
   node currently exists in the treap with key k. 
*/

void treap_delete(Treap_tree *t, Treap_node *node_to_delete);

Treap_tree *treap_create(void);

void treap_free(Treap_tree *t);

Disk *getDisk(Treap_node *n);

Treap_node *tree_search_bf(Treap_tree *tree, size_t size);