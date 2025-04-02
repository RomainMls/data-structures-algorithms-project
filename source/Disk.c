#include <stdlib.h>

#include "Disk.h"
#include "File.h"
#include "LinkedList.h"

struct Disk_t
{
    size_t used, free;
    List *files;
};

Disk *diskCreate(size_t diskSize)
{
    Disk *d = malloc(sizeof(Disk));
    if (!d)
    {
        return NULL;
    }
    d->free = diskSize;
    d->used = 0;
    d->files = llCreateEmpty();

    return d;
}

void diskFree(void *d) {
    Disk *dd = d;
    llFree(dd->files); // files are not freed.
    free(dd);
}

bool diskAddFile(Disk *d, File *f)
{
    if (d->free < fileSize(f))
    {
        return false;
    }

    llInsertLast(d->files, f);
    d->free -= fileSize(f);
    d->used += fileSize(f);
    return true;
}

size_t diskFreeSpace(const Disk *d)
{
    return d->free;
}

size_t diskUsedSpace(const Disk *d)
{
    return d->used;
}

List *diskFiles(Disk *d) {
    return d->files;
}

int compareDiskFreeSpace(const void *d1, const void *d2)
{
    return (int)diskFreeSpace(d1) - (int)diskFreeSpace(d2);
}
