#include <stdio.h>
#include <stdlib.h>

#define MAX_ALLOCATIONS 1024

static void *allocatedPointers[MAX_ALLOCATIONS];
static size_t numPointers = 0;

static void unregisterPointer(void *ptr)
{
    for (size_t i = 0; i < numPointers; ++i)
    {
        if (allocatedPointers[i] == ptr)
        {
            // Move the last pointer to the current position
            allocatedPointers[i] = allocatedPointers[--numPointers];
            return;
        }
    }
}

void cleanUpAllPointerMallocs()
{
    for (size_t i = 0; i < numPointers; ++i)
    {
        free(allocatedPointers[i]);
    }
    numPointers = 0;
}

void *safeMalloc(size_t size)
{
    if (numPointers < MAX_ALLOCATIONS)
    {
        void *ptr = malloc(size);
        if (!ptr && (size > 0))
        {
            fprintf(stderr, "Memory allocation failed\n");
            cleanUpAllPointerMallocs();
            exit(EXIT_FAILURE);
        }
        return ptr;
    }
    else
    {
        fprintf(stderr, "Memory allocation tracker is full\n");
        cleanUpAllPointerMallocs();
        exit(EXIT_FAILURE);
    }
}

void freePointer(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    unregisterPointer(ptr);
    free(ptr);
}
