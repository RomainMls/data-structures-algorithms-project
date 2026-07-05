#ifndef BP_H_DEFINED
#define BP_H_DEFINED

#include <stddef.h>
#include "LinkedList.h"
#include "File.h"

/**
 * @brief Returns the number of disks of size 'diskSize' required to store
 * the files in the list 'files' using a bin packing heuristic. The
 * created disks with the files should be returned through the list 'disks'
 * (initially assumed to be empty) given as the third argument. The files 
 * need to be directly stored on the disks (no copies are required).
 * 
 * @param diskSize The size of the disks
 * @param files The list of files (of type Files) to be stored
 * @param disks An empty list where the created disks should be stored.
 * @return * return, 
 */

size_t binpacking(size_t diskSize, List *files, List *disks);

#endif
