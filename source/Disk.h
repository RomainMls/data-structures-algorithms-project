
#ifndef  DISK_H_DEFINED
#define  DISK_H_DEFINED

#include <stddef.h>
#include <stdbool.h>
#include "LinkedList.h"
#include "File.h"

typedef struct Disk_t Disk;

/**
 * @brief Creates an empty disk.
 * 
 * @param diskSize the size of the disk
 * @return Disk* 
 */
Disk *diskCreate(size_t diskSize);

/**
 * @brief Frees a disk from memory. The list of files is freed but not the files.
 * 
 * @param d 
 */
void diskFree(void *d);

/**
 * @brief Adds a file to a disk.
 * 
 * @param d the disk
 * @param f the file
 * @return true if the file has been added to the disk
 * @return false if there is not enough room left on the disk
 */
bool diskAddFile(Disk *d, File *f);

/**
 * @brief Returns a list of the files on the disk
 * 
 * @param d the disk
 * @return List* the list of files
 */
List *diskFiles(Disk *d);

/**
 * @brief Returns the amount of free space on the disk
 * 
 * @param d the disk
 * @return size_t the amount of free space on the disk
 */
size_t diskFreeSpace(const Disk *d);

/**
 * @brief Returns the amount of used space on the disk
 * 
 * @param d the disk
 * @return size_t the amount of used space on the disk
 */
size_t diskUsedSpace(const Disk *d);

/**
 * @brief Compares two disks on their free space. Returns a negative (resp. positive) 
 * interger if the size of the first disk is greater (resp. lower) than the size of 
 * the second one. Returns 0 if they are equal.
 * 
 * @param d1 the first disk
 * @param d2 the second disk
 * @return int the result of the comparison
 */
int compareDiskFreeSpace(const void *d1, const void *d2);

#endif // DISK_H_DEFINED
