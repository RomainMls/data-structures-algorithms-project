
#include <stddef.h>
#include <stdlib.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"
#include "BST_bestfit.h"
/*
    Implementation of the Best-Fit strategy
*/

static int reverse_file_compare(const void *a, const void *b)
{
    return -compareFileSize(a, b);
}

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, reverse_file_compare);        // sorted in decreasing

    AVL_tree *avl = avl_create();
    if(avl == NULL)
        return (size_t)(0);

    size_t nbDisks = 0;

    Node *currentNode = llHead(files);
    while(currentNode != NULL)
    {
        File *currentFile = llData(currentNode);
        size_t size = fileSize(currentFile);
        if(size > diskSize)
        {
            printf("File size larger than disks' sizes\n");
            avl_free(avl);
            while(llPopFirst(disks) != NULL);   // reset disks list to match the 0
            return (size_t)(0);
        }

        Disk *diskToStoreIn;
        AVL_Node *n = tree_search_bf(avl, size);
        if(n != NULL)
        {
            diskToStoreIn = getDisk(n);
            //avl delete n
            if(!diskAddFile(diskToStoreIn, currentFile))
            {
                printf("BP_bestfit: can't add file to disk\n");
                exit(1);
            }
            avl_insert(avl, diskToStoreIn);
        }
        else
        {
            diskToStoreIn = diskCreate(diskSize);
            if(diskToStoreIn == NULL)
            {
                printf("BP_bestfit: allocation error.\n");
                exit(1);
            }
            nbDisks++;
            llInsertLast(disks, diskToStoreIn);
            avl_insert(avl, diskToStoreIn);
        }
        currentNode = llNext(currentNode);

        // if(detect_imbalance(avl))
        // {
        //     printf("BP_bestfit: imbalance detected\n");
        //     exit(1);
        // }
    }

    avl_free(avl);
    return nbDisks;
}