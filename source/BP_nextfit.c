
#include <stddef.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"

/*
    Implementation of the Next-Fit strategy
*/

static int compare(const void *a, const void *b){
    return (fileSize((File *)b) - fileSize((File *)a));
}

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, compare);
    if(llLength(files) == 0)
        return 0;

    Disk *currentDisk = diskCreate(diskSize);
    size_t nbDisks = 1;

    Node *p = llHead(files);

    while(p != NULL){
        File *f = llData(p);

        if(fileSize(f) <= diskFreeSpace(currentDisk)){
            diskAddFile(currentDisk, f);
        } else {
            //The file cannot be save on a disk, return error
            if(fileSize(f) > diskSize)
                return 0;
            
            llInsertLast(disks, currentDisk);
            currentDisk = diskCreate(diskSize);
            nbDisks++;
            diskAddFile(currentDisk, f);
        }

        p = llNext(p);
    }
    
    llInsertLast(disks, currentDisk);
    return nbDisks;
}
