
#include <stddef.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"

/*
    Implementation of the Next-Fit strategy
*/

static int compare(const void *a, const void *b){
    File *n1 = llData((Node *)a);
    File *n2 = llData((Node *)b);

    return (fileSize(n1) - fileSize(n2));
}

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, compare);

    if(llLength == 0)
        return 0;

    Disk *currentDisk = diskCreate(diskSize);
    size_t nbDisks = 1;

    Node *p = llHead(files);

    while(p != NULL){
        File *f = llData(p);

        if(fileSize(f) <= diskFreeSpace(currentDisk)){
            diskAddFile(currentDisk, f);

        } else {
            llInsertLast(disks, currentDisk);
            currentDisk = diskCreate(diskSize);
            nbDisks++;
        }

        p = llNext(p);
    }

    return 0;
}
