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

intptr_t
testTreeCmp
(uintptr_t left, uintptr_t right)
{
   return right - left;
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

   testTree = bTreeCreate(testTreeCmp);

   assert(testTree->rootNode == NULL);
   assert(testTree->cmp == testTreeCmp);

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

int
main
(int argc, char **argv)
{
   testList();
   testTree();

   return 0;
}
