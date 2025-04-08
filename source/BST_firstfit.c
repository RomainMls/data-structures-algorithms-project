#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "BST_firstfit.h"

#define ALLOW_DUPLICATES false

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
    int priority;
    Disk *disk;
    size_t subMax;
    struct BST_Node *parent;
    struct BST_Node *left;
    struct BST_Node *right;
    Balance_Factor bf;
}
BST_Node;

struct AVL_tree_t
{
    BST_Node *root;
    int counter;
};

AVL_tree *avl_create(void)
{
    AVL_tree *tree = malloc(sizeof(AVL_tree));
    if(tree == NULL)
        return NULL;

    tree->root = NULL;
    tree->counter = 0;

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

static int calculate_balance_factor(BST_Node *x)
{
    if(x == NULL)
        return BALANCED;

    return subtree_height(x->right) - subtree_height(x->left);
}

static size_t restore_sub_max(AVL_tree *tree, BST_Node *n)
{
    if(n == NULL)
        return 0;

    if(n->left == NULL && n->right == NULL)
    {
        n->subMax = diskFreeSpace(n->disk);
        return n->subMax;
    }

    size_t leftMax = restore_sub_max(tree, n->left);
    size_t rightMax = restore_sub_max(tree, n->right);

    if(n->left == NULL && n->right != NULL)
    {
        if(diskFreeSpace(n->disk) > rightMax)
        {
            n->subMax = diskFreeSpace(n->disk);
            return n->subMax;
        }
        else
        {
            n->subMax = rightMax;
            return rightMax;
        }
    }

    if(n->right == NULL && n->left != NULL)
    {
        if(diskFreeSpace(n->disk) > leftMax)
        {
            n->subMax = diskFreeSpace(n->disk);
            return n->subMax;
        }
        else
        {
            n->subMax = leftMax;
            return leftMax;
        }
    }

    if(leftMax > rightMax)
    {
        if(diskFreeSpace(n->disk) > leftMax)
        {
            n->subMax = diskFreeSpace(n->disk);
            return n->subMax;
        }
        else
        {
            n->subMax = leftMax;
            return leftMax;
        }
    }
    else
    {
        if(diskFreeSpace(n->disk) > rightMax)
        {
            n->subMax = diskFreeSpace(n->disk);
            return n->subMax;
        }
        else
        {
            n->subMax = rightMax;
            return rightMax;
        }
    }
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

bool avl_insert(AVL_tree *tree, Disk *disk)
{
    BST_Node *y = NULL;
    BST_Node *x = tree->root;
    BST_Node *z = malloc(sizeof(BST_Node));
    if(z == NULL)
        return false;

    z->disk = disk;
    z->left = NULL;
    z->right = NULL;
    z->bf = BALANCED;
    z->priority = ++(tree->counter);

    while(x != NULL)
    {
        y = x;
        x = x->right;
    }

    z->parent = y;
    if(y == NULL)
        tree->root = z;
    else
        y->right = z;

    BST_Node *n = z->parent;
    while(n != NULL)
    {
        Balance_Factor bf = calculate_balance_factor(n);

        if(bf == RIGHTIMBALANCE)
        {
            if(calculate_balance_factor(n->right) >= BALANCED)
            {
                n = left_rotate(tree, n);
                restore_sub_max(tree, n->parent);
            }
            else
            {
                n->right = right_rotate(tree, n->right);
                n = left_rotate(tree, n);
                restore_sub_max(tree, n->parent);
            }
        }
        else if(bf == LEFTIMBALANCE)
        {
            if(calculate_balance_factor(n->left) <= BALANCED)
            {   n = right_rotate(tree, n);
                restore_sub_max(tree, n->parent);
            }
            else
            {
                n->left = left_rotate(tree, n->left);
                n = right_rotate(tree, n);
                restore_sub_max(tree, n->parent);
            }
        }
        n = n->parent;
    }
    restore_sub_max(tree, z);
    return true;
}

static Disk *tree_search_ff_node(BST_Node *root, size_t size)
{
    if(root == NULL)
        return NULL;

    if(root->subMax < size)
        return NULL;

    BST_Node *currentBest = root;

    while(currentBest->left != NULL && currentBest->left->subMax >= size)
        currentBest = currentBest->left;

    if(diskFreeSpace(currentBest->disk) >= size)
        return currentBest->disk;
    else
        return tree_search_ff_node(currentBest->right, size);
}

Disk *tree_search_ff(AVL_tree *tree, size_t size)
{
    return tree_search_ff_node(tree->root, size);
}

void avl_restore_sub_max(AVL_tree *tree)
{
    restore_sub_max(tree, tree->root);
}

bool detect_imbalance(AVL_tree *tree)
{
    if(abs(calculate_balance_factor(tree->root)) > 1)
        return true;

    return false;
}