#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <boring.h>

#pragma comment(lib, "libboring")

void
testList
(void)
{
   list_t *list;
   list_entry_t *entry;

   list = bListCreate();

   assert(list != NULL);

   entry = bListPushBack(list, 5);

   assert(list->front == list->back);

   entry = bListPushBack(list, 2);

   assert(list->front != list->back);
   assert(list->back->prev->label == 5);

   entry = bListPushFront(list, 1);

   assert(list->back->prev->prev->label == 1);

   entry = bListPopBack(list);

   assert(entry->label == 2);
   assert(list->back->label == 5);

   bFree(entry);
   entry = bListInsertBefore(list->back, 3);

   assert(list->back->label != 3);
   assert(list->back->prev->label == 3);

   entry = bListInsertAfter(list->front, 10);

   assert(list->front->label != 10);
   assert(list->front->next->label == 10);

   /* current state:
      1 -> 10 -> 3 -> 5 */

   assert(bListMoveDown(list->back) == false);
   assert(bListMoveUp(list->front) == false);

   assert(bListMoveUp(list->back->prev) == true);
   assert(list->back->prev->label == 10);

   assert(bListMoveDown(list->front->next) == true);
   assert(list->front->next->label == 10);

   bListDestroy(list);
}

void
testTree
(void)
{
   tree_t *testTree;
   node_t *testNode;

   /*           5
              /   \
            2      10
          /   \   /  \
         1     3 7    14 */

   testTree = bTreeCreate(bTreeBinCmp);

   assert(testTree->rootNode == NULL);
   assert(testTree->cmp == bTreeBinCmp);

   testNode = bTreeInsert(testTree, 5);

   assert(testNode == testTree->rootNode);
   assert(testNode->label == 5);
   assert(testNode->left == NULL);
   assert(testNode->right == NULL);
   assert(testNode->parent == NULL);

   testNode = bTreeInsert(testTree, 10);

   assert(testNode != NULL);
   assert(testNode->label == 10);
   assert(testNode->parent == testTree->rootNode);
   assert(testNode->parent->right == testNode);

   testNode = bTreeInsert(testTree, 2);

   assert(testNode != NULL);
   assert(testNode->label == 2);
   assert(testNode->parent == testTree->rootNode);
   assert(testNode->parent->left == testNode);

   testNode = bTreeInsert(testTree, 1);

   assert(testNode != NULL);
   assert(testNode->label == 1);
   assert(testTree->rootNode->left->left == testNode);

   testNode = bTreeInsert(testTree, 3);

   assert(testNode != NULL);
   assert(testNode->label == 3);
   assert(testTree->rootNode->left->right == testNode);

   testNode = bTreeInsert(testTree, 7);

   assert(testNode != NULL);
   assert(testNode->label == 7);
   assert(testTree->rootNode->right->left == testNode);

   testNode = bTreeInsert(testTree, 14);

   assert(testNode != NULL);
   assert(testNode->label == 14);
   assert(testTree->rootNode->right->right == testNode);

   bNodeDelete(testTree->rootNode->left->left); /* leaf case */

   assert(testTree->rootNode->left->left == NULL);

   bNodeDelete(testTree->rootNode->left); /* single-child case */

   assert(testTree->rootNode->left != NULL);
   assert(testTree->rootNode->left->label == 3);

   bNodeDelete(testTree->rootNode); /* full-node case */

   assert(testTree->rootNode->label == 10);
   assert(testTree->rootNode->left->right != NULL);
   assert(testTree->rootNode->left->right->label == 7);

   bTreeFree(testTree);
}

void
testAVL
(void)
{
   tree_t *avlTree;
   node_t *testNode;

   avlTree = bTreeCreate(bAVLBinCmp);

   testNode = bAVLInsert(avlTree, 5);

   assert(bNodeGetAVL(testNode)->label == 5);

   testNode = bAVLInsert(avlTree, 10);

   assert(bNodeGetAVL(testNode)->label == 10);
   assert(bNodeGetAVL(testNode->parent)->label == 5);

   /* single left rotation */
   testNode = bAVLInsert(avlTree, 14);

   assert(bNodeGetAVL(avlTree->rootNode)->label == 10);
   assert(bNodeGetAVL(avlTree->rootNode->left)->label == 5);
   assert(bNodeGetAVL(avlTree->rootNode->right)->label == 14);

   /*          10
              /   \
            5      14       */

   /* single right rotation */
   testNode = bAVLInsert(avlTree, 3);
   bAVLInsert(avlTree, 2);

   assert(bNodeGetAVL(testNode->parent)->label == 10);
   assert(bNodeGetAVL(testNode->right)->label == 5);
   assert(bNodeGetAVL(testNode->left)->label == 2);
   assert(bNodeGetAVL(testNode)->label == 3);

   /*          10
              /   \
            3      14       
          /   \
         2     5          */

   /* double rotation */
   testNode = bAVLInsert(avlTree, 6);

   /* steps:

               10
              /   \
            3      14       
          /   \
         2     5          
                \
                 6


               10
              /   \
            5      14       
          /   \
         3     6          
        /       
       2
              
            5 
          /   \
         3     10          
        /     /  \
       2     6    14   */

   assert(bNodeGetAVL(testNode->parent)->label == 10);
   assert(bNodeGetAVL(testNode->parent->parent)->label == 5);
   assert(testNode->parent->parent == avlTree->rootNode);

   bAVLTreeFree(avlTree);
}

void
testHash
(void)
{
   hash_table_t *table;
   hash_entry_t *entry;

   table = bHashTableCreate(BORING_HASH_SMALL, bHashString);

   bHashTableSet(table, "one", 1);
   bHashTableSet(table, "two", 2);
   bHashTableSet(table, "three", 3);
   bHashTableSet(table, "four", 4);
   bHashTableSet(table, "five", 5);
   bHashTableSet(table, "six", 6);
   bHashTableSet(table, "seven", 7);
   bHashTableSet(table, "eight", 8);
   bHashTableSet(table, "nine", 9);
   bHashTableSet(table, "ten", 10);
   bHashTableSet(table, "eleven", 11);
   bHashTableSet(table, "twelve", 12);

   entry = bHashTableGet(table, "one");

   assert(entry != NULL);
   assert(entry->label == 1);

   entry = bHashTableGet(table, "two");

   assert(entry != NULL);
   assert(entry->label == 2);

   entry = bHashTableGet(table, "three");

   assert(entry != NULL);
   assert(entry->label == 3);

   entry = bHashTableGet(table, "four");

   assert(entry != NULL);
   assert(entry->label == 4);

   entry = bHashTableGet(table, "thirteen");

   assert(entry == NULL);

   bHashTableSet(table, "one", 0);
   entry = bHashTableGet(table, "one");

   assert(entry->label != 1);
   assert(entry->label == 0);

   bHashTableFree(table);
}

int
main
(int argc, char **argv)
{
   testList();
   testTree();
   testAVL();
   testHash();

   return 0;
}
