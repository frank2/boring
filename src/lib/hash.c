#include <boring/hash.h>

uint64_t
bHashFNV
(void *data, size_t size)
{
   static uint64_t offsetBasis = 14695981039346656037;
   static uint64_t prime = 1099511628211;

   uint64_t result = offsetBasis;
   uint8_t *buffer = (uint8_t *)data;
   size_t iter;

   for (iter=0; iter<size; ++iter)
   {
      result *= prime;
      result ^= buffer[iter];
   }

   return result;
}

hash_table_t *
bHashTableCreate
(uint8_t size, hash_key_t hasher)
{
   hash_table_t *table;
   uint64_t iter, expandedSize;
   
   assert(size <= 32);

   expandedSize = 1ull << size;

   table = bAlloc(sizeof(hash_table_t));
   table->size = size;
   table->hasher = hasher;
   table->buckets = bAlloc(sizeof(list_t *) * expandedSize);

   for (iter=0; iter<expandedSize; ++iter)
      table->buckets[iter] = bListCreate();

   return table;
}

void
bHashTableDestroy
(hash_table_t *table)
{
   list_t **buckets;
   uint64_t bucketCount, iter;
   
   assert(table != NULL);

   bucketCount = 1ull << table->size;
   buckets = table->buckets;

   for (iter=0; iter<bucketCount; ++iter)
      bListDestroy(buckets[iter]);
}

void
bHashTableFree
(hash_table_t *table)
{
   uint64_t iter, expandedSize;

   assert(table != NULL);

   bHashTableDestroy(table);

   expandedSize = 1ull << table->size;

   for (iter=0; iter<expandedSize; ++iter)
      bListFree(table->buckets[iter]);

   bFree(table->buckets);
   
   table->size = 0;
   
   bFree(table);
}

hash_entry_t *
bHashTableSet
(hash_table_t *table, void *key, uintptr_t value)
{
   uint64_t hashKey, truncKey;
   list_t *list;
   hash_entry_t *entry;
   
   assert(table != NULL);
   entry = bHashTableGet(table, key);

   /* hash was found-- overwrite the label */
   if (entry != NULL)
   {
      entry->label = value;
      return entry;
   }

   /* the hash wasn't found in the bucket-- add it */
   hashKey = table->hasher(key);
   truncKey = hashKey % (1ull << table->size);

   list = table->buckets[truncKey];

   entry = bAlloc(sizeof(hash_entry_t));
   entry->key = hashKey;
   entry->label = value;

   bListPushBack(list, (uintptr_t)entry);

   return entry;
}

hash_entry_t *
bHashTableGet
(hash_table_t *table, void *key)
{
   uint64_t hashKey, truncKey;
   list_t *list;
   list_entry_t *entry;
   
   assert(table != NULL);

   hashKey = table->hasher(key);
   truncKey = hashKey % (1ull << table->size);
   list = table->buckets[truncKey];
   entry = list->front;

   while (entry != NULL)
   {
      hash_entry_t *hashEntry;

      hashEntry = (hash_entry_t *)entry->label;

      if (hashEntry->key == hashKey)
         return hashEntry;

      entry = entry->next;
   }

   return NULL;
}

uint64_t
bHashString
(char *str)
{
   assert(str != NULL);

   return bHashFNV(str, strlen(str));
}

uint64_t
bHashUnicode
(wchar_t *str)
{
   assert(str != NULL);

   return bHashFNV(str, wcslen(str)*2);
}
