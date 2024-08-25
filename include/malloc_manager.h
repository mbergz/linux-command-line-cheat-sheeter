#ifndef UTILS_H
#define UTILS_H

void *safeMalloc(size_t size);
void *safeMallocWithCleanup(size_t size, void *cleanUpPtr);
void freePointer(void *ptr);
void cleanUpAllPointerMallocs();

#endif