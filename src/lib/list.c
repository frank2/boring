#include <boring/list.h>

list_t *
bListCreate
(void)
{
   list_t *result;

   result = bAlloc(sizeof(list_t));
   result->front = NULL;
   result->back = NULL;

   return result;
}

void
bListDestroy
(list_t *list)
{
   list_entry_t *entry;

   assert(list != NULL);

   entry = list->front;

   if (entry == NULL)
      return;

   while (entry->next != NULL)
   {
      entry = entry->next;
      bFree(entry->prev);
      entry->prev = NULL;
   }

   bFree(entry);

   list->front = NULL;
   list->back = NULL;
}

void
bListFree
(list_t *list)
{
   assert(list != NULL);
   
   bListDestroy(list);
   bFree(list);
}

list_entry_t *
bListPushBack
(list_t *list, uintptr_t label)
{
   list_entry_t *entry;

   assert(list != NULL);

   entry = bAlloc(sizeof(list_entry_t));
   entry->label = label;

   if (list->front == NULL && list->back == NULL)
      list->front = list->back = entry;
   else
   {
      list->back->next = entry;
      entry->prev = list->back;
      list->back = entry;
   }

   entry->list = list;

   return entry;
}

list_entry_t *
bListPopBack
(list_t *list)
{
   list_entry_t *entry, *prev;

   assert(list != NULL);

   entry = list->back;

   if (entry == NULL)
      return NULL;

   prev = entry->prev;

   if (prev != NULL)
   {
      list->back = prev;
      prev->next = NULL;
   }
   else
      list->front = list->back = NULL;

   entry->next = NULL;
   entry->prev = NULL;
   entry->list = NULL;

   return entry;
}

list_entry_t *
bListPushFront
(list_t *list, uintptr_t label)
{
   list_entry_t *entry;

   assert(list != NULL);

   entry = bAlloc(sizeof(list_entry_t));
   entry->label = label;

   if (list->front == NULL && list->back == NULL)
      list->front = list->back = entry;
   else
   {
      list->front->prev = entry;
      entry->next = list->front;
      list->front = entry;
   }

   entry->list = list;

   return entry;
}

list_entry_t *
bListPopFront
(list_t *list)
{
   list_entry_t *entry, *next;

   assert(list != NULL);

   entry = list->front;

   if (entry == NULL)
      return NULL;

   next = entry->next;

   if (next != NULL)
   {
      list->front = next;
      next->prev = NULL;
   }
   else
      list->front = list->back = NULL;

   entry->next = NULL;
   entry->prev = NULL;
   entry->list = NULL;

   return entry;
}

list_entry_t *
bListInsertBefore
(list_entry_t *listEntry, uintptr_t newItem)
{
   list_t *list;
   list_entry_t *prev, *entry;
   
   assert(listEntry != NULL);
   assert(listEntry->list != NULL);

   list = listEntry->list;

   if (listEntry == list->front)
      return bListPushFront(list, newItem);

   prev = listEntry->prev;
   entry = bAlloc(sizeof(list_entry_t));

   entry->label = newItem;
   entry->list = list;
   entry->next = listEntry;

   if (prev != NULL)
   {
      prev->next = entry;
      entry->prev = prev;
      listEntry->prev = entry;
   }

   return entry;
}

list_entry_t *
bListInsertAfter
(list_entry_t *listEntry, uintptr_t newItem)
{
   list_t *list;
   list_entry_t *next, *entry;
   
   assert(listEntry != NULL);

   list = listEntry->list;

   if (listEntry == list->back)
      return bListPushBack(list, newItem);

   next = listEntry->next;
   entry = bAlloc(sizeof(list_entry_t));

   entry->label = newItem;
   entry->list = list;
   entry->prev = listEntry;

   if (next != NULL)
   {
      next->prev = entry;
      entry->next = next;
      listEntry->next = entry;
   }

   return entry;
}

bool
bListMoveUp
(list_entry_t *listEntry)
{
   list_entry_t *swapPrev, *swapPPrev, *swapNext;
   
   assert(listEntry != NULL);
   assert(listEntry->list != NULL);

   /* visuals:

      pPrev -> prev -> entry -> next -> nNext

      the goal:

      pPrev -> entry -> prev -> next -> nNext

      in this scenario, nNext is irrelevant
   */

   if (listEntry->list->front == listEntry)
      return false;

   swapPrev = listEntry->prev;
   swapPPrev = swapPrev->prev;
   swapNext = listEntry->next;
   
   swapPrev->next = swapNext;
   swapPrev->prev = listEntry;
   
   listEntry->next = swapPrev;
   listEntry->prev = swapPPrev;

   swapNext->prev = swapPrev;

   if (swapPPrev != NULL)
      swapPPrev->next = listEntry;
   else
      listEntry->list->front = listEntry;

   return true;
}

bool
bListMoveDown
(list_entry_t *listEntry)
{
   list_entry_t *swapPrev, *swapNext, *swapNNext;
   
   assert(listEntry != NULL);
   assert(listEntry->list != NULL);

   /* visuals:

      pPrev -> prev -> entry -> next -> nNext

      the goal:

      pPrev -> prev -> next -> entry -> nNext

      in this scenario, pPrev is irrelevant
   */

   if (listEntry->list->back == listEntry)
      return false;

   swapPrev = listEntry->prev;
   swapNext = listEntry->next;
   swapNNext = swapNext->next;
   
   swapPrev->next = swapNext;
   
   listEntry->next = swapNNext;
   listEntry->prev = swapNext;

   swapNext->prev = swapPrev;
   swapNext->next = listEntry;

   if (swapNNext != NULL)
      swapNNext->prev = listEntry;
   else
      listEntry->list->back = listEntry;

   return true;
}

void
bListEntryFree
(list_entry_t *list)
{
   assert(list != NULL);

   bFree(list);
}

list_entry_t *
bListFind
(list_t *list, uintptr_t label)
{
   list_entry_t *iter;

   assert(list != NULL);

   iter = list->front;

   while (iter != NULL)
   {
      if (iter->label == label)
         return iter;

      iter = iter->next;
   }

   return NULL;
}
