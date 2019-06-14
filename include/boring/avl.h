#ifndef __BORING_AVL_H
#define __BORING_AVL_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include <boring/alloc.h>
#include <boring/tree.h>

typedef struct __avl_label_t
{
   uintptr_t height;
   uintptr_t label;
} avl_label_t;

#define bNodeGetAVL(node) ((avl_label_t *)node->label)

void bAVLRotateLeft(node_t *node);
void bAVLRotateRight(node_t *node);
void bAVLUpdateHeight(node_t *node);
void bAVLRebalance(node_t *node);
node_t *bAVLInsert(tree_t *tree, uintptr_t label);

void bAVLNodeDelete(node_t *node);
void bAVLTreeDestroy(node_t *node);
void bAVLTreeFree(tree_t *tree);

intptr_t bAVLBinCmp(uintptr_t left, uintptr_t right);

#endif
