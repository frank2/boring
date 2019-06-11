#ifndef __BORING_LIST_H
#define __BORING_LIST_H

#include <assert.h>
#include <stdint.h>

#include <boring/alloc.h>

typedef struct __list_t
{
   struct __list_entry_t *front, *back;
} list_t;

typedef struct __list_entry_t
{
   list_t *list;
   uintptr_t label;
   struct __list_entry_t *prev, *next;
} list_entry_t;

list_t *bListCreate(void);
void bListDestroy(list_t *list);
void bListFree(list_t *list);

list_entry_t *bListPushBack(list_t *list, uintptr_t label);
list_entry_t *bListPopBack(list_t *list);

list_entry_t *bListPushFront(list_t *list, uintptr_t label);
list_entry_t *bListPopFront(list_t *list);

list_entry_t *bListInsertBefore(list_entry_t *listEntry, uintptr_t newItem);
list_entry_t *bListInsertAfter(list_entry_t *listEntry, uintptr_t newItem);

bool bListMoveUp(list_entry_t *listEntry);
bool bListMoveDown(list_entry_t *listEntry);

void bListEntryFree(list_entry_t *listEntry);

list_entry_t *bListFind(list_t *list, uintptr_t label);

#endif
