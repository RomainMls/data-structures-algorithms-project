
#include <stddef.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"
#include "BST_firstfit.h"
#include <stdio.h>

/*
    Implementation of the First-Fit strategy
*/

static int reverse_file_compare(const void *a, const void *b)
{
    return -compareFileSize(a, b);
}

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, reverse_file_compare);
    if(llLength(files) == 0)
        return 0;

    AVL_tree *avl = avl_create();
    if(avl == NULL)
        return (size_t)(0);

    size_t counter = 0;
    Node *currentNode = llHead(files);
    File *currentFile;
    Disk *diskToStoreIn;
    int i = 0;
    while(currentNode != NULL)
    {
        i++;

        currentFile = llData(currentNode);
        size_t size = fileSize(currentFile);
        //printf("Starting iteration %d, with file size = %zu\n", i, size);
        if(size > diskSize)
        {
            printf("File too big to fit in disk size\n");
            return (size_t)(0);
        }
        diskToStoreIn = tree_search_ff(avl, size);
        if(diskToStoreIn == NULL)
        {
            diskToStoreIn = diskCreate(diskSize);
            if(diskToStoreIn == NULL)
            {
                return (size_t)(0);
            }
            //printf("New disk\n");
            avl_insert(avl, diskToStoreIn);
            counter++;
            llInsertLast(disks, diskToStoreIn);
        }
        diskAddFile(diskToStoreIn, currentFile);
        avl_restore_sub_max(avl);
        // available size has changed but avl did not record it, subMax is therefore no more reliable
        // a solution is to make a new function 'restore subMax' from the root of the tree

        currentNode = llNext(currentNode);
        /*
        printf("Printing tree at end of iteration %d\n", i);
        avl_print(avl);
        printf("\n\n");
        */
    }

    return counter;
}

