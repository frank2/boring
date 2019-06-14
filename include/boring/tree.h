#ifndef __BORING_TREE_H
#define __BORING_TREE_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include <boring/alloc.h>
#include <boring/list.h>

typedef intptr_t (*tree_cmp_t)(uintptr_t left, uintptr_t right);

typedef struct __tree_t
{
   struct __node_t *rootNode;
   tree_cmp_t cmp;
} tree_t;

typedef struct __node_t
{
   tree_t *tree;
   uintptr_t label;
   struct __node_t *left, *right, *parent;
} node_t;

tree_t *bTreeCreate(tree_cmp_t cmp);
node_t *bNodeCreate(tree_t *tree, uintptr_t label, node_t *left
                    ,node_t *right, node_t *parent);

intptr_t bTreeLabelCmp(tree_t *tree, uintptr_t left, uintptr_t right);
intptr_t bNodeCmp(node_t *left, node_t *right);

bool bNodeIsRoot(node_t *node);
bool bNodeIsLeft(node_t *node);
bool bNodeIsRight(node_t *node);
bool bNodeIsLeaf(node_t *node);
bool bNodeIsFull(node_t *node);

void bNodeReRoot(node_t *node, node_t *root, bool side);
node_t *bTreeInsert(tree_t *tree, uintptr_t label);
node_t *bTreeFind(tree_t *tree, uintptr_t label);

void bNodeDelete(node_t *node);
void bTreeDestroy(node_t *node);
void bTreeFree(tree_t *tree);

intptr_t bTreeBinCmp(uintptr_t left, uintptr_t right);

#endif
