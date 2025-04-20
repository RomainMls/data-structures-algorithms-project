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

    Treap_tree *treap = treap_create();
    if(treap == NULL)
        return 0;

    size_t nbDisks = 0;
    Node *currentNode = llHead(files);

    while(currentNode != NULL)
    {
        File *currentFile = llData(currentNode);
        size_t size = fileSize(currentFile);

        if(size > diskSize)
        {
            printf("File size larger than disks' sizes\n");
            treap_free(treap);
            while(llPopFirst(disks) != NULL);   // reset disks list to match the 0
            return 0;
        }

        Disk *diskToStoreIn = NULL;
        Treap_node *n = tree_search_bf(treap, size);
        if(n != NULL)
        {
            diskToStoreIn = getDisk(n);
            treap_delete(treap, n);
        }
        else
        {
            diskToStoreIn = diskCreate(diskSize);
            if(diskToStoreIn == NULL)
            {
                printf("BP_bestfit: allocation error.\n");
                treap_free(treap);
                return 0;
            }

            nbDisks++;
            llInsertLast(disks, diskToStoreIn);
        }

        if (!diskAddFile(diskToStoreIn, currentFile))
        {
            printf("BP_bestfit: can't add file to disk.\n");
            return 0;
        }

        treap_insert(treap, diskToStoreIn);
        currentNode = llNext(currentNode);
    }

    treap_free(treap);
    return nbDisks;
}
