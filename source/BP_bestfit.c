
#include <stddef.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"
#include "BST_bestfit.h"
#include <stdlib.h>
/*
    Implementation of the Best-Fit strategy
*/

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, compareFileSize);
    AVL_tree *avl = create_avl(compareDiskFreeSpace, diskFree);
    if(avl == NULL)
        return (size_t)(-1);

    Node *currentNode = llHead(files);
    File *currentFile;
    Disk *diskToStoreIn;
    int count = 0;
    while(currentNode != NULL)
    {
        currentFile = llData(currentNode);

        Disk *fakeDisk = diskCreate(fileSize(currentFile) - 1);     // fake disk that would be the best fit

        diskToStoreIn = avl_successor(avl, fakeDisk);  // disk of minimum free size that can store our file
        if(diskToStoreIn == NULL)
        {
            // No disk can store the file, therefore add a new disk
            diskToStoreIn = diskCreate(diskSize);
            if(diskToStoreIn == NULL)
            {
                free(avl);
                return (size_t)(-1);
            }
            count++;
            avl_insert(avl, diskToStoreIn);
            llInsertLast(disks, diskToStoreIn);
        }
        avl_delete_without_free(avl, diskToStoreIn);
        diskAddFile(diskToStoreIn, currentFile);
        avl_insert(avl, diskToStoreIn);
        diskFree(fakeDisk);

        currentNode = llNext(currentNode);
    }

    return count;
}
