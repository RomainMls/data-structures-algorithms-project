
#include <stddef.h>
#include <stdlib.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"
#include "BST_firstfit.h"
/*
    Implementation of the First-Fit strategy
*/

static int reverse_file_compare(const void *a, const void *b)
{
    return -compareFileSize(a, b);
}

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, reverse_file_compare);        // sorted in decreasing

    if(llLength(files) == 0)
        return 0;

    AVL_tree *avl = avl_create();
    if(avl == NULL)
        return (size_t)(0);

    size_t nbDisks = 0;

    Node *currentNode = llHead(files);
    File *currentFile;

    while(currentNode != NULL)
    {
        currentFile = llData(currentNode);
        size_t size = fileSize(currentFile);
        if(size > diskSize)
        {
            avl_free_with_freeDisk(avl);
            printf("File size larger than disks' sizes\n");
            while(llPopFirst(disks) != NULL);   // reset disks list to match the 0
            return (size_t)(0);
        }

        AVL_Node *resultNode = tree_search_ff(avl, size);
        Disk *diskToStoreIn;
        if(resultNode != NULL)
        {
            diskToStoreIn = getDisk(resultNode);
            size_t prev_size = diskFreeSpace(diskToStoreIn);
            if(!diskAddFile(diskToStoreIn, currentFile))
            {
                printf("BP_bestfit: can't add file to disk\n");
                exit(1);
            }
            avl_notify_update(avl, resultNode, prev_size);
        }
        else
        {
            diskToStoreIn = diskCreate(diskSize);
            if(diskToStoreIn == NULL)
            {
                printf("BP_firstfit: allocation error.\n");
                exit(1);
            }
            nbDisks++;
            llInsertLast(disks, diskToStoreIn);
            if(!diskAddFile(diskToStoreIn, currentFile))
            {
                printf("BP_bestfit: can't add file to disk\n");
                exit(1);
            }
            avl_insert(avl, diskToStoreIn);
        }

        currentNode = llNext(currentNode);

        if(detect_imbalance(avl))
        {
            printf("BP_bestfit: imbalance detected\n");
            exit(1);
        }
    }

    avl_free_without_freeDisk(avl);
    return nbDisks;
}


