#include <stdio.h>
#include <stdlib.h>
#include "BST_firstfit.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"
#include "BP.h"

static int reverse_file_compare(const void *a, const void *b)
{
    return -compareFileSize(a, b);
}

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, reverse_file_compare); // sort files in decreasing order

    if (llLength(files) == 0)
        return 0;

    Treap_tree *treap = treap_create();
    if (treap == NULL)
        return 0;

    size_t nbDisks = 0;

    Node *currentNode = llHead(files);
    File *currentFile = NULL;

    while (currentNode != NULL)
    {
        currentFile = llData(currentNode);
        size_t size = fileSize(currentFile);
        if (size > diskSize)
        {
            treap_free(treap);
            printf("File size larger than disk size.\n");
            while (llPopFirst(disks) != NULL);
            return 0;
        }

        Treap_node *resultNode = tree_search_ff(treap, size);
        Disk *diskToStoreIn = NULL;
        if (resultNode != NULL)
        {
            diskToStoreIn = getDisk(resultNode);

            size_t prev_size = diskFreeSpace(diskToStoreIn);
            if (!diskAddFile(diskToStoreIn, currentFile))
            {
                printf("BP_firstfit: can't add file to disk\n");
                exit(1);
            }
            treap_notify(resultNode, prev_size);
        }
        else
        {
            diskToStoreIn = diskCreate(diskSize);
            if (diskToStoreIn == NULL)
            {
                printf("BP_firstfit: allocation error.\n");
                treap_free(treap);
                return 0;
            }
            nbDisks++;
            llInsertLast(disks, diskToStoreIn);

            if (!diskAddFile(diskToStoreIn, currentFile))
            {
                printf("BP_firstfit: can't add file to disk\n");
                exit(1);
            }
            treap_insert(treap, diskToStoreIn);
        }

        currentNode = llNext(currentNode);
    }

    treap_free(treap);
    return nbDisks;
}
