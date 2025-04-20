#include <stdio.h>
#include <stdlib.h>

#include "BST_firstfit.h"
#include "Disk.h"

static size_t creation_counter = 0;

struct Treap_node_t {
    Disk *disk;
    int priority;
    size_t creation_order;
    size_t subMax;
    struct Treap_node_t *parent;
    struct Treap_node_t *left;
    struct Treap_node_t *right;
};

struct Treap_tree_t {
    struct Treap_node_t *root;
};

static size_t redefine_node_submax(Treap_node *n) {
    if(n == NULL)
        return 0;
    size_t l = 0, r = 0, temp_max = 0;

    if(n->left != NULL)
        l = n->left->subMax;
    if(n->right != NULL)
        r = n->right->subMax;

    if(l > r)
        temp_max = l;
    else
        temp_max = r;
    
    if(temp_max > diskFreeSpace(n->disk))
        return temp_max;
    else
        return diskFreeSpace(n->disk);
}

static void update_submax_branch(Treap_node *n) {
    while(n != NULL){
        n->subMax = redefine_node_submax(n);
        n = n->parent;
    }
}

static void sift_upwards (Treap_tree *t, Treap_node *q)
/* On entry, t is an ordered tree, where the only node violating the
   property is q.  Moreover, the subtree rooted at q is a treap.
   Rotates q upwards until the treap property is restored for t.
*/
{
  Treap_node *p, *n;

  while ((q->parent != NULL) && (q->parent->priority < q->priority)) {
    p = q->parent;
    q->parent = p->parent;
    p->parent = q;

    if (q->parent != NULL) {
      if (q->parent->left == p)
	q->parent->left = q;
      else
	q->parent->right = q;
    } else
      t->root = q;

/*  rotation à droite      rotation à gauche
     avant  après             avant  après 
    p 54     q 46           p 54          q 56
   / \      / \            / \           / \
  q 46         p 54           q 56      p 54
 / \          / \            / \       / \
    n 49     n 49        67 n             n 67
   / \      / \            / \           / \    
On voit bien que le changement des submax à faire est local et sur p et q
*/
    if (q == p->left) {  /* right rotation */
      n = q->right;
      q->right  = p;
      p->left   = n;
    } else { /* left rotation */
      n = q->left;
      q->left = p;
      p->right = n;
    }
    if (n != NULL) n->parent = p;

    p->subMax = redefine_node_submax(p);
    q->subMax = redefine_node_submax(q);
  }
}

void treap_insert
  (Treap_tree *t, Disk *d)
/* Inserts the pair (k,d) into the treap t. */
{
  Treap_node *n = t->root;    /* The current node we are examining in
				   our path down through the tree. */
  Treap_node *q = NULL;  /* The new node which we are inserting
				   into the tree. */

  q = (Treap_node *) malloc (sizeof(Treap_node));
  q->left = q->right = q->parent = NULL;
  q->creation_order = creation_counter++;
  q->subMax = diskFreeSpace(d);
  q->disk = d;
  q->priority = rand();

  /* If the treap is initially empty, then we treat this as a special
     case: */
  
  if (t->root == NULL) {
    t->root = q;
    return;
  }

  /* Now we search down through the treap, looking for where to insert
     our node: */
  while (n != NULL) {
    q->parent = n;
    if (q->creation_order < n->creation_order)
        n = n->left;
    else
        n = n->right;
  }

  if (q->parent->creation_order > q->creation_order)
    q->parent->left = q;
  else
    q->parent->right = q;

  /* At this point, q has been inserted into the tree as a leaf,
     and the ordering of the keys of the tree has been maintained.
     We now need to move q upwards through the tree in order to
     restore the heap order.
     */
  update_submax_branch(q->parent); //Re actualiser les submax
  sift_upwards (t, q); //Refaire le treap correctement
}


void treap_delete 
  (Treap_tree *t, Treap_node *p)
/* Searches the treap t for an element whose key matches *k.
   If found, deletes the element, sets *k equal to the key pointer in
   that that element, and returns the associated data.  If not found,
   returns NULL.
*/
{
  Treap_node *n, *q;    /* descendants of p. */

  /* At this point, p points to our node.  We must push it down to a 
     leaf, and then we can detach the leaf. */
  while ((p->left != NULL) && (p->right != NULL)) {
    if (p->left->priority > p->right->priority) { /* right rotation */
      q = p->left;
      n = q->right;
      q->right = p;
      p->left = n;
    } else { /* left rotation */
      q = p->right;
      n = q->left;
      q->left = p;
      p->right = n;
    }
    if (p->parent != NULL) {
      if (p->parent->left == p)
	p->parent->left = q;
      else
	p->parent->right = q;
    } else
      t->root = q;
    q->parent = p->parent;
    p->parent = q;
    if (n != NULL) n->parent = p;
  }

  /* At this point, either p's left child is NULL or p's right child
     is NULL, so we can safely remove p from the tree. */
  if (p->left == NULL)
    q = p->right;
  else
    q = p->left;
  if (q != NULL) q->parent = p->parent;

  if (p->parent != NULL) {
    if (p == p->parent->left)
      p->parent->left = q;
    else
      p->parent->right = q;
    update_submax_branch(p->parent); 
    //Comme on pousse le noeud a supprimé jusqu'au fond de la branche sans impacter d'autres branches, on 
    //a qu'à refaire les submax de cette branche
  } else
    t->root = q;

  free(p);
}


Treap_tree *treap_create(void){
    Treap_tree *t = malloc(sizeof(Treap_tree));
    if(t == NULL)
        return NULL;
    
    t->root = NULL;
    return t;
}

static void treap_free_subtree(Treap_node *n){
    if(n == NULL)
        return;
    treap_free_subtree(n->left);
    treap_free_subtree(n->right);
    free(n);
}

void treap_free(Treap_tree *t){
    if(t == NULL)
        return;
    treap_free_subtree(t->root);
}

Disk *getDisk(Treap_node *n){
    if(n == NULL)
        return NULL;
    return n->disk;
}


static Treap_node *tree_search_ff_node(Treap_node *root, size_t size)
{
    if(root == NULL)
        return NULL;

    if(root->subMax < size)
        return NULL;

    Treap_node *currentBest = root;

    while(currentBest->left != NULL && currentBest->left->subMax >= size)
        currentBest = currentBest->left;

    if(diskFreeSpace(currentBest->disk) >= size)
        return currentBest;
    else
        return tree_search_ff_node(currentBest->right, size);
}

Treap_node *tree_search_ff(Treap_tree *tree, size_t size)
{
    return tree_search_ff_node(tree->root, size);
}
