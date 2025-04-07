
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

    Node *currentNode = llHead(files);
    File *currentFile;
    Disk *diskToStoreIn;
    size_t counter = 0;

    while(currentNode != NULL)
    {
        currentFile = llData(currentNode);
        size_t size = fileSize(currentFile);
        if(size > diskSize)
        {
            printf("File size larger than disks' sizes\n");
            avl_free_without_freeDisk(avl);
            while(llPopFirst(disks) != NULL);   // reset disks list to match the 0
            return (size_t)(0);
        }

        diskToStoreIn = tree_search_bf(avl, fileSize(currentFile));
        if(diskToStoreIn == NULL)
        {
            // No disk can store the file, therefore add a new disk
            diskToStoreIn = diskCreate(diskSize);
            if(diskToStoreIn == NULL)
            {
                printf("BP_bestfit: allocation error.\n");
                exit(1);
            }
            counter++;
            avl_insert(avl, diskToStoreIn);
            llInsertLast(disks, diskToStoreIn);
        }
        avl_delete_without_free(avl, diskToStoreIn);
        diskAddFile(diskToStoreIn, currentFile);
        avl_insert(avl, diskToStoreIn);

        currentNode = llNext(currentNode);
    }

    avl_free_without_freeDisk(avl);
    return counter;
}