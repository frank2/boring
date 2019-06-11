#ifndef __BORING_ALLOC_H
#define __BORING_ALLOC_H

#include <stdbool.h>
#include <stdint.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <stdlib.h>
#endif

void *bAlloc(size_t size);
void *bRealloc(void *ptr, size_t size);
bool bFree(void *ptr);

#endif
