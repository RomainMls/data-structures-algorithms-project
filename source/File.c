
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "File.h"

struct File_t
{
    char *name;
    size_t size;
};

File *fileCreate(char *name, size_t size)
{
    File *f = malloc(sizeof(File));
    if (!f)
    {
        fprintf(stderr, "fileCreate: allocation error.\n");
        exit(1);
    }
    f->name = name;
    f->size = size;
    return f;
}

void fileFree(void *f)
{
    free(f);
}

size_t fileSize(const File *f)
{
    return f->size;
}

char *fileName(const File *f)
{
    return f->name;
}

int compareFileSize(const void *f1, const void *f2)
{
    return (int)fileSize(f1) - (int)fileSize(f2);
}
