#include "BST_bestfit.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define ALLOW_DUPLICATES false
#define MAINTAIN_MIN true

typedef enum
{
    LEFTIMBALANCE = -2,
    LEFTHEAVY = -1,
    BALANCED = 0,
    RIGHTHEAVY = 1,
    RIGHTIMBALANCE = 2
}
Balance_Factor;

typedef struct BST_Node
{
    void *key;
    struct BST_Node *parent;
    struct BST_Node *left;
    struct BST_Node *right;
    Balance_Factor bf;
}
BST_Node;

struct AVL_tree_t
{
    BST_Node *root;
    BST_Node *min;
    int (*compare)(const void *, const void *);
    void (*freeKey)(void *);
};

static void free_subtree(AVL_tree *tree, BST_Node *root);
static int subtree_height(BST_Node *root);
static int max(int a, int b);
static BST_Node *find_node(AVL_tree *tree, void *key);
static BST_Node *avl_successor_node(AVL_tree *tree, BST_Node *node);
static int calculate_balance_factor(BST_Node *x);
static void transplant(AVL_tree *tree, BST_Node *a, BST_Node *b);
static void tree_delete(AVL_tree *tree, BST_Node *z);
static BST_Node *left_rotate(AVL_tree *tree, BST_Node *x);
static BST_Node *right_rotate(AVL_tree *tree, BST_Node *x);
static void update_min(AVL_tree *tree);
static void *avl_find_min(AVL_tree *tree);

AVL_tree *create_avl(int (*compare)(const void *, const void *), void (*freeKey)(void *))
{
    AVL_tree *tree = malloc(sizeof(AVL_tree));
    if(tree == NULL)
        return NULL;

    tree->compare = compare;
    tree->freeKey = freeKey;
    tree->root = NULL;
    tree->min = NULL;

    return tree;
}

void free_avl(AVL_tree *tree)
{
    free_subtree(tree, tree->root);
    free(tree);
}

static void free_subtree(AVL_tree *tree, BST_Node *root)
{
    if(root == NULL)
        return;

    free_subtree(tree, root->left);
    free_subtree(tree, root->right);

    tree->freeKey(root->key);
    free(root);
}

int avl_height(AVL_tree *tree)
{
    return subtree_height(tree->root);
}

static int subtree_height(BST_Node *root)
{
    if(root == NULL)
        return 0;

    return 1 + max(subtree_height(root->left), subtree_height(root->right));
}

static int max(int a, int b)
{
    if(a > b)
        return a;

    return b;
}

static BST_Node *find_node(AVL_tree *tree, void *key)
{
    BST_Node *ptr = tree->root;

    while(ptr != NULL)
    {
        int cmp = tree->compare(key, ptr->key);
        if(cmp == 0)
            return ptr;

        if(cmp > 0)
            ptr = ptr->right;
        else
            ptr = ptr->left;
    }

    return NULL;
}

static BST_Node *avl_successor_node(AVL_tree *tree, BST_Node *node)
{
    BST_Node *ptr = node->right;

    if(ptr != NULL) {
        while(ptr->left != NULL)
            ptr = ptr->left;
        return ptr;
    }
    else
    {
        ptr = node->parent;
        BST_Node *current = node;

        while(ptr != NULL && current == ptr->right)
        {
            current = ptr;
            ptr = ptr->parent;
        }

        return ptr;
    }
}

void *avl_successor(AVL_tree *tree, void *key)
{
    if (tree == NULL || key == NULL)
        return NULL;

    // First, find the node for the given key
    BST_Node *current = tree->root;
    BST_Node *successor = NULL;
    while (current != NULL)
    {
        int cmp = tree->compare(key, current->key);

        if (cmp < 0)
        {
            // Current node's key is larger than our key,
            // so it's a potential successor
            successor = current;
            current = current->left;
        }
        else
            // Either equal or smaller, go right
            current = current->right;
    }

    if(successor == NULL)
        return NULL;

    return successor->key;
}

static void update_min(AVL_tree *tree)
{
    if (tree->root == NULL)
    {
        tree->min = NULL;
        return;
    }

    BST_Node *current = tree->root;
    while(current->left != NULL)
        current = current->left;

    tree->min = current;
}

bool avl_insert(AVL_tree *tree, void *key)
{
    BST_Node *y = NULL;
    BST_Node *x = tree->root;
    BST_Node *z = malloc(sizeof(BST_Node));
    if(z == NULL)
        return false;

    z->key = key;
    z->left = NULL;
    z->right = NULL;
    z->bf = BALANCED;

    while(x != NULL)
    {
        y = x;

        int cmp = tree->compare(key, x->key);
        if(!ALLOW_DUPLICATES && cmp == 0) {
            // Key already exists and duplicates not allowed
            free(z);
            return false;
        }

        if(cmp < 0)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if(y == NULL)
        tree->root = z;
    else
    {
        if(tree->compare(key, y->key) < 0)
            y->left = z;
        else
            y->right = z;
    }

    if(MAINTAIN_MIN)
        if(tree->min == NULL || tree->compare(key, tree->min->key) < 0)
            tree->min = z;

    // now we need to rebalance from the z to the root
    BST_Node *n = z->parent;
    while(n != NULL)
    {
        Balance_Factor bf = calculate_balance_factor(n);

        if(bf == RIGHTIMBALANCE)
        {
            if(calculate_balance_factor(n->right) >= BALANCED)
                n = left_rotate(tree, n);
            else
            {
                n->right = right_rotate(tree, n->right);
                n = left_rotate(tree, n);
            }
        }
        else if(bf == LEFTIMBALANCE)
        {
            if(calculate_balance_factor(n->left) <= BALANCED)
                n = right_rotate(tree, n);
            else
            {
                n->left = left_rotate(tree, n->left);
                n = right_rotate(tree, n);
            }
        }
        n = n->parent;
    }

    return true;
}

bool avl_delete_with_free(AVL_tree *tree, void *key)
{
    BST_Node *z = find_node(tree, key);
    if(z == NULL)
        return false;

    // Save parent node to start rebalancing from
    BST_Node *parent = z->parent;

    tree_delete(tree, z);
    tree->freeKey(z->key);
    free(z);

    // Update minimum if necessary
    if(MAINTAIN_MIN)
        update_min(tree);

    // Rebalance the tree from the parent node upward
    BST_Node *n = parent;
    while(n != NULL)
    {
        Balance_Factor bf = calculate_balance_factor(n);

        if(bf == RIGHTIMBALANCE)
        {
            if(calculate_balance_factor(n->right) >= BALANCED)
                n = left_rotate(tree, n);
            else
            {
                n->right = right_rotate(tree, n->right);
                n = left_rotate(tree, n);
            }
        }
        else if(bf == LEFTIMBALANCE)
        {
            if(calculate_balance_factor(n->left) <= BALANCED)
                n = right_rotate(tree, n);
            else
            {
                n->left = left_rotate(tree, n->left);
                n = right_rotate(tree, n);
            }
        }
        n = n->parent;
    }

    return true;
}

bool avl_delete_without_free(AVL_tree *tree, void *key)
{
    BST_Node *z = find_node(tree, key);
    if(z == NULL)
        return false;

    // Save parent node to start rebalancing from
    BST_Node *parent = z->parent;

    tree_delete(tree, z);
    //tree->freeKey(z->key);
    free(z);

    // Update minimum if necessary
    if(MAINTAIN_MIN)
        update_min(tree);

    // Rebalance the tree from the parent node upward
    BST_Node *n = parent;
    while(n != NULL)
    {
        Balance_Factor bf = calculate_balance_factor(n);

        if(bf == RIGHTIMBALANCE)
        {
            if(calculate_balance_factor(n->right) >= BALANCED)
                n = left_rotate(tree, n);
            else
            {
                n->right = right_rotate(tree, n->right);
                n = left_rotate(tree, n);
            }
        }
        else if(bf == LEFTIMBALANCE)
        {
            if(calculate_balance_factor(n->left) <= BALANCED)
                n = right_rotate(tree, n);
            else
            {
                n->left = left_rotate(tree, n->left);
                n = right_rotate(tree, n);
            }
        }
        n = n->parent;
    }

    return true;
}

static int calculate_balance_factor(BST_Node *x)
{
    if(x == NULL)
        return BALANCED;

    return subtree_height(x->right) - subtree_height(x->left);
}

static void transplant(AVL_tree *tree, BST_Node *a, BST_Node *b)
{
    if(a->parent == NULL)
        tree->root = b;
    else
    {
        if(a == a->parent->left)
            a->parent->left = b;
        else
            a->parent->right = b;
    }
    if(b != NULL)
        b->parent = a->parent;
}

static void tree_delete(AVL_tree *tree, BST_Node *z)
{
    if(z->left == NULL)
        transplant(tree, z, z->right);
    else
    {
        if(z->right == NULL)
            transplant(tree, z, z->left);
        else
        {
            BST_Node *y = avl_successor_node(tree, z);
            if(y->parent != z)
            {
                transplant(tree, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(tree, z, y);
            y->left = z->left;
            y->left->parent = y;
        }
    }
}

static BST_Node *left_rotate(AVL_tree *tree, BST_Node *x)
{
    if(x == NULL || x->right == NULL)
        return x;

    BST_Node *y = x->right;

    // Update parent pointers
    y->parent = x->parent;
    if(x->parent == NULL)
        tree->root = y;
    else
    {
        if(x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }

    // Perform rotation
    x->right = y->left;
    if(y->left != NULL)
        y->left->parent = x;

    y->left = x;
    x->parent = y;

    // Update balance factors
    x->bf = calculate_balance_factor(x);
    y->bf = calculate_balance_factor(y);

    return y;
}

static BST_Node *right_rotate(AVL_tree *tree, BST_Node *x)
{
    if(x == NULL || x->left == NULL)
        return x;

    BST_Node *y = x->left;

    // Update parent pointers
    y->parent = x->parent;
    if(x->parent == NULL)
        tree->root = y;
    else
    {
        if(x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }

    // Perform rotation
    x->left = y->right;
    if(y->right != NULL)
        y->right->parent = x;

    y->right = x;
    x->parent = y;

    // Update balance factors
    x->bf = calculate_balance_factor(x);
    y->bf = calculate_balance_factor(y);

    return y;
}

static void *avl_find_min(AVL_tree *tree)
{
    if(tree->min != NULL && MAINTAIN_MIN)
        return tree->min->key;

    if(tree->root == NULL)
        return NULL;

    BST_Node *current = tree->root;
    while(current->left != NULL)
        current = current->left;

    return current->key;
}

void *avl_find(AVL_tree *tree, void *key)
{
    BST_Node *node = find_node(tree, key);
    return node != NULL ? node->key : NULL;
}

void *avl_get_min(AVL_tree *tree)
{
    return tree->min->key;
}

