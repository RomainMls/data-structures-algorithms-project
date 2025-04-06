
#include <stddef.h>

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
    while(currentNode != NULL)
    {
        currentFile = llData(currentNode);
        size_t size = fileSize(currentFile);
        if(size > diskSize)
        {
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
            avl_insert(avl, diskToStoreIn);
            counter++;
            llInsertLast(disks, diskToStoreIn);
        }
        diskAddFile(diskToStoreIn, currentFile);

        currentNode = llNext(currentNode);
    }

    return counter;
}

