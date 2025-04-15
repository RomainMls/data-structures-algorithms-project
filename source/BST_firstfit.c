#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "BST_firstfit.h"

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

static void free_subtree_with_freeDisk(AVL_tree *tree, AVL_Node *root)
{
    if(root == NULL)
        return;

    free_subtree_with_freeDisk(tree, root->left);
    free_subtree_with_freeDisk(tree, root->right);

    diskFree(root->disk);
    free(root);
}

static void free_subtree_without_freeDisk(AVL_tree *tree, AVL_Node *root)
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

static int subtree_height(AVL_Node *root)
{
    if(root == NULL)
        return 0;

    return 1 + max(subtree_height(root->left), subtree_height(root->right));
}

int avl_height(AVL_tree *tree)
{
    return subtree_height(tree->root);
}

static int calculate_balance_factor(AVL_Node *x)
{
    if(x == NULL)
        return BALANCED;

    return subtree_height(x->right) - subtree_height(x->left);
}

static size_t restore_subMax(AVL_tree *tree, AVL_Node *n)
{
    if(n == NULL)
        return 0;

    if(n->left == NULL && n->right == NULL)
    {
        n->subMax = diskFreeSpace(n->disk);
        return n->subMax;
    }

    size_t leftMax = restore_subMax(tree, n->left);
    size_t rightMax = restore_subMax(tree, n->right);

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

    return n->subMax;
}

static AVL_Node *left_rotate(AVL_tree *tree, AVL_Node *x)
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

    x->bf = calculate_balance_factor(x);
    y->bf = calculate_balance_factor(y);

    return y;
}

static AVL_Node *right_rotate(AVL_tree *tree, AVL_Node *x)
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

    x->bf = calculate_balance_factor(x);
    y->bf = calculate_balance_factor(y);

    return y;
}

bool avl_insert(AVL_tree *tree, Disk *disk)
{
    AVL_Node *y = NULL;
    AVL_Node *x = tree->root;
    AVL_Node *z = malloc(sizeof(AVL_Node));
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

    AVL_Node *n = z->parent;
    while(n != NULL)
    {
        Balance_Factor bf = calculate_balance_factor(n);

        if(bf == RIGHTIMBALANCE)
        {
            if(calculate_balance_factor(n->right) >= BALANCED)
            {
                n = left_rotate(tree, n);
                restore_subMax(tree, n->parent);
            }
            else
            {
                n->right = right_rotate(tree, n->right);
                n = left_rotate(tree, n);
                restore_subMax(tree, n->parent);
            }
        }
        else if(bf == LEFTIMBALANCE)
        {
            if(calculate_balance_factor(n->left) <= BALANCED)
            {   n = right_rotate(tree, n);
                restore_subMax(tree, n->parent);
            }
            else
            {
                n->left = left_rotate(tree, n->left);
                n = right_rotate(tree, n);
                restore_subMax(tree, n->parent);
            }
        }
        n = n->parent;
    }
    restore_subMax(tree, z);
    return true;
}

static AVL_Node *tree_search_ff_node(AVL_Node *root, size_t size)
{
    if(root == NULL)
        return NULL;

    if(root->subMax < size)
        return NULL;

    AVL_Node *currentBest = root;

    while(currentBest->left != NULL && currentBest->left->subMax >= size)
        currentBest = currentBest->left;

    if(diskFreeSpace(currentBest->disk) >= size)
        return currentBest;
    else
        return tree_search_ff_node(currentBest->right, size);
}

AVL_Node *tree_search_ff(AVL_tree *tree, size_t size)
{
    return tree_search_ff_node(tree->root, size);
}

void avl_notify_update(AVL_tree *tree, AVL_Node *modified_node, size_t prev_size)
{
    /*  Nodes to be updates:
        (Their subMax can only decrease since the disk size has decreased)
        The node that contains the disk given as argument
        and all of its parents to the root
    */
    if(modified_node == NULL)
        return;

    if(modified_node->subMax == prev_size)
        restore_subMax(tree, modified_node);

    else
        // modification has no impact on the subMax structure
        return;

    AVL_Node *current = modified_node->parent;

    while(current != NULL)
    {
        if(current->subMax == prev_size)
        {
            // that means that the submax was probably due to the modified file
            // we should update it
            restore_subMax(tree, current);
        }
        else
            // if the previous size wasn't the submax for current
            // it isn't for its parent
            break;

        current = current->parent;
    }
}

bool detect_imbalance(AVL_tree *tree)
{
    if(abs(calculate_balance_factor(tree->root)) > 1)
        return true;

    return false;
}