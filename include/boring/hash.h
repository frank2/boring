#ifndef __BORING_HASH_H
#define __BORING_HASH_H

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

#include <boring/alloc.h>
#include <boring/list.h>

#define BORING_HASH_SMALL 8
#define BORING_HASH_MEDIUM 16
#define BORING_HASH_LARGE 32

typedef uint64_t (*hash_key_t)(void *key);

typedef struct __hash_table_t
{
   uint8_t size; /* size of the hashtable in bits */
   list_t **buckets;
   hash_key_t hasher;
} hash_table_t;

typedef struct __hash_entry_t
{
   uint64_t key;
   uintptr_t label;
} hash_entry_t;

uint64_t bHashFNV(void *data, size_t size);

hash_table_t *bHashTableCreate(uint8_t size, hash_key_t hasher);
void bHashTableDestroy(hash_table_t *table);
void bHashTableFree(hash_table_t *table);

hash_entry_t *bHashTableSet(hash_table_t *table, void *key, uintptr_t value);
hash_entry_t *bHashTableGet(hash_table_t *table, void *key);

uint64_t bHashString(char *str);
uint64_t bHashUnicode(wchar_t *str);

#endif
