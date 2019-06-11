#include <boring/alloc.h>

void *
bAlloc
(size_t size)
{
#ifdef WIN32
   return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
#else
   return malloc(size);
#endif
}

void *
bRealloc
(void *ptr, size_t size)
{
#ifdef WIN32
   return HeapReAlloc(GetProcessHeap(), 0, ptr, size);
#else
   return realloc(ptr, size);
#endif
}

bool
bFree
(void *ptr)
{
#ifdef WIN32
   return HeapFree(GetProcessHeap(), 0, ptr);
#else
   free(ptr);
   return true;
#endif
}
