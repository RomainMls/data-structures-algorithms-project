
#include <stddef.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"
#include "PQ.h"

/*
    Implementation of the Worst-Fit strategy
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

    PQ *pq = pqCreate(llLength(files), compareDiskFreeSpace);
    size_t nbDisks = 0;

    Node *p = llHead(files);
    while(p != NULL){
        File *f = llData(p);

        Disk *mostFreeDisk = pqGetMax(pq);
        if(mostFreeDisk != NULL && fileSize(f) <= diskFreeSpace(mostFreeDisk)){
            diskAddFile(mostFreeDisk, f);

            //Reorganize pq with the new value
            pqExtractMax(pq);
            pqInsert(pq, mostFreeDisk);

        } else {
            //The file cannot be save on a disk, return error
            if(fileSize(f) > diskSize)
                return 0;

            Disk *newDisk = diskCreate(diskSize);
            diskAddFile(newDisk, f);
            pqInsert(pq, newDisk);
            llInsertLast(disks, newDisk);
            nbDisks++;
        }
        p = llNext(p);
    }

    pqFree(pq);
    return nbDisks;
}
