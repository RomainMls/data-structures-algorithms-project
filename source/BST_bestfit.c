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

struct AVL_Node_t
{
    int priority;
    Disk *disk;
    size_t subMax;
    AVL_Node *parent;
    AVL_Node *left;
    AVL_Node *right;
    Balance_Factor bf;
};

Disk *getDisk(AVL_Node *n)
{
    if(n != NULL)
        return n->disk;

    return NULL;
}

struct AVL_tree_t
{
    AVL_Node *root;
};

AVL_tree *avl_create(void)
{
    AVL_tree *tree = malloc(sizeof(AVL_tree));
    if(tree == NULL)
        return NULL;

    tree->root = NULL;

    return tree;
}

static void free_subtree(AVL_tree *tree, AVL_Node *root)
{
    if(root == NULL)
        return;

    free_subtree(tree, root->left);
    free_subtree(tree, root->right);

    free(root);
}

void avl_free(AVL_tree *tree)
{
    free_subtree(tree, tree->root);
    free(tree);
}

static int max(int a, int b)
{
    if(a > b)
        return a;

    return b;
}

static int subtree_height(AVL_Node *root)
{
    if(root == NULL)
        return 0;

    return 1 + max(subtree_height(root->left), subtree_height(root->right));
}

static AVL_Node *avl_successor_node(AVL_tree *tree, AVL_Node *node)
{
    AVL_Node *ptr = node->right;

    if(ptr != NULL) {
        while(ptr->left != NULL)
            ptr = ptr->left;
        return ptr;
    }
    else
    {
        ptr = node->parent;
        AVL_Node *current = node;

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

static int recalculate_balance_factor(AVL_Node *x)
{
    if(x == NULL)
        return BALANCED;

    return subtree_height(x->right) - subtree_height(x->left);
}

static AVL_Node *rotate_left(AVL_tree *tree, AVL_Node *x)
{
    if(x == NULL || x->right == NULL)
        return x;

    AVL_Node *y = x->right;

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

    return y;
}

static AVL_Node *rotate_right(AVL_tree *tree, AVL_Node *x)
{
    if(x == NULL || x->left == NULL)
        return x;

    AVL_Node *y = x->left;

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

    return y;
}

AVL_Node *avl_insert(AVL_tree *tree, Disk *d)
{
    AVL_Node *y = NULL;
    AVL_Node *x = tree->root;
    AVL_Node *z = malloc(sizeof(AVL_Node));
    if(z == NULL)
        return NULL;

    z->disk = d;
    z->left = NULL;
    z->right = NULL;
    z->bf = BALANCED;       // because the node will end up at the bottom of the tree

    while(x != NULL)
    {
        y = x;

        int cmp = compareDiskFreeSpace(d, x->disk);

        if(cmp < 0)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if(y == NULL)
    {
        tree->root = z;
        return z;
    }
    else
    {
        if(compareDiskFreeSpace(d, y->disk) < 0)
            y->left = z;
        else
            y->right = z;
    }

    // node successfully added, but we need to rebalance the tree
    return z;
}

static void transplant(AVL_tree *tree, AVL_Node *a, AVL_Node *b)
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

static void delete_node(AVL_tree *tree, AVL_Node *z)
{
    if(z->left == NULL)
        transplant(tree, z, z->right);
    else
    {
        if(z->right == NULL)
            transplant(tree, z, z->left);
        else
        {
            AVL_Node *y = avl_successor_node(tree, z);
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

    free(z);
}

void avl_delete(AVL_tree *tree, AVL_Node *n)
{
    delete_node(tree, n);
}

AVL_Node *tree_search_bf(AVL_tree *tree, size_t size)
{
    if (tree == NULL)
        return NULL;

    AVL_Node *current = tree->root;
    AVL_Node *successor = NULL;
    while (current != NULL)
    {
        int cmp = size - diskFreeSpace(current->disk);
        if(cmp == 0)
            return current;

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

    return successor;
}

bool detect_imbalance(AVL_tree *tree)
{
    if(abs(recalculate_balance_factor(tree->root)) > 1)
        return true;

    return false;
}
