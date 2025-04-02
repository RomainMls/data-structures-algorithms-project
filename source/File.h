#ifndef FILE_H_DEFINED
#define FILE_H_DEFINED

#include <stddef.h>

typedef struct File_t File;

/**
 * @brief Creates a file
 * 
 * @param fileName the name of the file
 * @param fileSize the size of the file
 * @return File* a pointer to the created file
 */
File *fileCreate(char *fileName, size_t fileSize);

/**
 * @brief Frees file f from memory
 * 
 * @param f the file
 */
void fileFree(void *f);

/**
 * @brief Returns the size of the file f
 * 
 * @param f the file
 * @return size_t the size of the file
 */
size_t fileSize(const File *f);

/**
 * @brief Returns the name of the file f
 * 
 * @param f the file
 * @return char* the name of the file
 */
char *fileName(const File *f);

/**
 * @brief Compares two files according to their size. Returns a negative (resp. positive) 
 * interger if the size of the first file is greater (resp. lower) than the size of the
 * second one. Returns 0 if they are equal.
 *
 * @param f1 the first file
 * @param f2 the second file
 * @return int the result of the comparison
 */
int compareFileSize(const void *f1, const void *f2);

#endif
