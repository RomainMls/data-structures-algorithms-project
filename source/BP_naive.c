
#include <stddef.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"

/*
    A very suboptimal bin packing heuristic: each file is stored on a different disk.
*/

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    Node *p = llHead(files);

    size_t nbDisks = 0;

    while (p != NULL)
    {
        File *f = llData(p);
        
        Disk *currentDisk = diskCreate(diskSize);
        nbDisks++;
        diskAddFile(currentDisk, f);

        llInsertLast(disks, currentDisk);

        p = llNext(p);
    }

    return nbDisks;
}
