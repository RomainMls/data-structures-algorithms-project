#include <stdlib.h>
#include <stdbool.h>

#include "BST_bestfit.h"

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
    Disk *disk;
    struct BST_Node *parent;
    struct BST_Node *left;
    struct BST_Node *right;
    Balance_Factor bf;
}
BST_Node;

struct AVL_tree_t
{
    BST_Node *root;
};

AVL_tree *avl_create(void)
{
    AVL_tree *tree = malloc(sizeof(AVL_tree));
    if(tree == NULL)
        return NULL;

    tree->root = NULL;

    return tree;
}

static void free_subtree_with_freeDisk(AVL_tree *tree, BST_Node *root)
{
    if(root == NULL)
        return;

    free_subtree_with_freeDisk(tree, root->left);
    free_subtree_with_freeDisk(tree, root->right);

    diskFree(root->disk);
    free(root);
}

static void free_subtree_without_freeDisk(AVL_tree *tree, BST_Node *root)
{
    if(root == NULL)
        return;

    free_subtree_without_freeDisk(tree, root->left);
    free_subtree_without_freeDisk(tree, root->right);

    free(root);
}

void avl_free_with_freeDisk(AVL_tree *tree)
{
    free_subtree_with_freeDisk(tree, tree->root);
    free(tree);
}

void avl_free_without_freeDisk(AVL_tree *tree)
{
    free_subtree_without_freeDisk(tree, tree->root);
    free(tree);
}

static int max(int a, int b)
{
    if(a > b)
        return a;

    return b;
}

static int subtree_height(BST_Node *root)
{
    if(root == NULL)
        return 0;

    return 1 + max(subtree_height(root->left), subtree_height(root->right));
}

int avl_height(AVL_tree *tree)
{
    return subtree_height(tree->root);
}

static BST_Node *find_node(AVL_tree *tree, Disk *d)
{
    BST_Node *ptr = tree->root;

    while(ptr != NULL)
    {
        int cmp = compareDiskFreeSpace(d, ptr->disk);
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

        if(ptr == tree->root)
            return NULL;

        return ptr;
    }
}

static int calculate_balance_factor(BST_Node *x)
{
    if(x == NULL)
        return BALANCED;

    return subtree_height(x->right) - subtree_height(x->left);
}

static BST_Node *left_rotate(AVL_tree *tree, BST_Node *x)
{
    if(x == NULL || x->right == NULL)
        return x;

    BST_Node *y = x->right;

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

    x->right = y->left;
    if(y->left != NULL)
        y->left->parent = x;

    y->left = x;
    x->parent = y;

    x->bf = calculate_balance_factor(x);
    y->bf = calculate_balance_factor(y);

    return y;
}

static BST_Node *right_rotate(AVL_tree *tree, BST_Node *x)
{
    if(x == NULL || x->left == NULL)
        return x;

    BST_Node *y = x->left;

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

    x->left = y->right;
    if(y->right != NULL)
        y->right->parent = x;

    y->right = x;
    x->parent = y;

    x->bf = calculate_balance_factor(x);
    y->bf = calculate_balance_factor(y);

    return y;
}

bool avl_insert(AVL_tree *tree, Disk *d)
{
    BST_Node *y = NULL;
    BST_Node *x = tree->root;
    BST_Node *z = malloc(sizeof(BST_Node));
    if(z == NULL)
        return false;

    z->disk = d;
    z->left = NULL;
    z->right = NULL;
    z->bf = BALANCED;

    while(x != NULL)
    {
        y = x;

        int cmp = compareDiskFreeSpace(d, x->disk);
        if(cmp == 0)
        {
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
        if(compareDiskFreeSpace(d, y->disk) < 0)
            y->left = z;
        else
            y->right = z;
    }

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

bool avl_delete_with_free(AVL_tree *tree, Disk *d)
{
    BST_Node *z = find_node(tree, d);
    if(z == NULL)
        return false;

    BST_Node *parent = z->parent;

    tree_delete(tree, z);
    diskFree(z->disk);
    free(z);

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

bool avl_delete_without_free(AVL_tree *tree, Disk *d)
{
    BST_Node *z = find_node(tree, d);
    if(z == NULL)
        return false;

    BST_Node *parent = z->parent;

    tree_delete(tree, z);
    free(z);

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

Disk *avl_find(AVL_tree *tree, Disk *d)
{
    BST_Node *node = find_node(tree, d);
    if(node == NULL)
        return NULL;

    return node->disk;
}

Disk *tree_search_bf(AVL_tree *tree, size_t size)
{
    if (tree == NULL)
        return NULL;

    BST_Node *current = tree->root;
    BST_Node *successor = NULL;
    while (current != NULL)
    {
        int cmp = size - diskFreeSpace(current->disk);
        if(cmp == 0)
            return current->disk;

        if (cmp < 0)
        {
            successor = current;
            current = current->left;
        }
        else
            current = current->right;
    }

    if(successor == NULL)
        return NULL;

    return successor->disk;
}
