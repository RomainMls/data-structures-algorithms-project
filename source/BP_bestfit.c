
#include <stddef.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"
#include "BST_bestfit.h"
/*
    Implementation of the Best-Fit strategy
*/

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, compareFileSize);
    AVL_tree *avl = create_avl(compareDiskFreeSpace, diskFree);

    Disk *firstDisk = diskCreate(diskSize);
    if(!avl_insert(avl, firstDisk))
        return -1;

    Node *currentNode;
    File *currentFile;
    Disk *diskToStoreIn;
    while((currentNode = llPopFirst(files)) != NULL)
    {
        currentFile = llData(currentNode);
        diskToStoreIn = avl_successor(avl, currentFile);
    }
}
