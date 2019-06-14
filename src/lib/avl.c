#include <boring/avl.h>

void
bAVLRotateLeft
(node_t *node)
{
   node_t *pivotPoint, *leftChild, *rotationParent;
   tree_t *tree;
   uintptr_t leftHeight, rightHeight;
   
   assert(node != NULL);

   pivotPoint = node->right;
   leftChild = pivotPoint->left;
   rotationParent = node->parent;
   tree = node->tree;

   if (rotationParent == NULL)
      tree->rootNode = pivotPoint;
   else if (rotationParent->left == node)
      rotationParent->left = pivotPoint;
   else if (rotationParent->right == node)
      rotationParent->right = pivotPoint;

   pivotPoint->parent = rotationParent;

   if (leftChild != NULL)
      leftChild->parent = node;

   node->right = leftChild;
   pivotPoint->left = node;
   node->parent = pivotPoint;

   if (node->left == NULL)
      leftHeight = 0;
   else
      leftHeight = bNodeGetAVL(node->left)->height;

   if (node->right == NULL)
      rightHeight = 0;
   else
      rightHeight = bNodeGetAVL(node->right)->height;

   bNodeGetAVL(node)->height = ((leftHeight > rightHeight)
                                ? leftHeight : rightHeight)+1;

   if (pivotPoint->left == NULL)
      leftHeight = 0;
   else
      leftHeight = bNodeGetAVL(pivotPoint->left)->height;

   if (pivotPoint->right == NULL)
      rightHeight = 0;
   else
      rightHeight = bNodeGetAVL(pivotPoint->right)->height;

   bNodeGetAVL(pivotPoint)->height = ((leftHeight > rightHeight)
                                      ? leftHeight : rightHeight)+1;
}

void
bAVLRotateRight
(node_t *node)
{
   node_t *pivotPoint, *rightChild, *rotationParent;
   tree_t *tree;
   uintptr_t leftHeight, rightHeight;
   
   assert(node != NULL);

   pivotPoint = node->left;
   rightChild = pivotPoint->right;
   rotationParent = node->parent;
   tree = node->tree;

   if (rotationParent == NULL)
      tree->rootNode = pivotPoint;
   else if (rotationParent->left == node)
      rotationParent->left = pivotPoint;
   else if (rotationParent->right == node)
      rotationParent->right = pivotPoint;

   pivotPoint->parent = rotationParent;

   if (rightChild != NULL)
      rightChild->parent = node;

   node->left = rightChild;
   pivotPoint->right = node;
   node->parent = pivotPoint;

   if (node->left == NULL)
      leftHeight = 0;
   else
      leftHeight = bNodeGetAVL(node->left)->height;

   if (node->right == NULL)
      rightHeight = 0;
   else
      rightHeight = bNodeGetAVL(node->right)->height;

   bNodeGetAVL(node)->height = ((leftHeight > rightHeight)
                                ? leftHeight : rightHeight)+1;

   if (pivotPoint->left == NULL)
      leftHeight = 0;
   else
      leftHeight = bNodeGetAVL(pivotPoint->left)->height;
   
   if (pivotPoint->right == NULL)
      rightHeight = 0;
   else
      rightHeight = bNodeGetAVL(pivotPoint->right)->height;

   bNodeGetAVL(pivotPoint)->height = ((leftHeight > rightHeight) ?
                                      leftHeight : rightHeight)+1;
}

void
bAVLUpdateHeight
(node_t *node)
{
   assert(node != NULL);

   while (node != NULL)
   {
      uintptr_t leftHeight, rightHeight, oldHeight;
      intptr_t balance;
      
      if (node->left == NULL)
         leftHeight = 0;
      else
         leftHeight = bNodeGetAVL(node->left)->height;

      if (node->right == NULL)
         rightHeight = 0;
      else
         rightHeight = bNodeGetAVL(node->right)->height;

      oldHeight = bNodeGetAVL(node)->height;
      bNodeGetAVL(node)->height = ((leftHeight > rightHeight) ?
                                   leftHeight : rightHeight)+1;

      balance = rightHeight - leftHeight;

      if (balance < -1 || balance > 1)
      {
         bAVLRebalance(node);
         return;
      }

      if (oldHeight == bNodeGetAVL(node)->height)
         return;

      node = node->parent;
   }
}

void
bAVLRebalance
(node_t *node)
{
   uintptr_t leftHeight, rightHeight;
   intptr_t parentBalance, childBalance;
   node_t *childNode;
   
   assert(node != NULL);

   if (node->left == NULL)
      leftHeight = 0;
   else
      leftHeight = bNodeGetAVL(node->left)->height;

   if (node->right == NULL)
      rightHeight = 0;
   else
      rightHeight = bNodeGetAVL(node->right)->height;

   parentBalance = rightHeight - leftHeight;

   if (parentBalance > 0)
      childNode = node->right;
   else if (parentBalance < 0)
      childNode = node->left;
   else
      return;

   if (childNode != NULL)
   {
      if (childNode->left == NULL)
         leftHeight = 0;
      else
         leftHeight = bNodeGetAVL(childNode->left)->height;

      if (childNode->right == NULL)
         rightHeight = 0;
      else
         rightHeight = bNodeGetAVL(childNode->right)->height;

      childBalance = rightHeight - leftHeight;
   }
   else
      childBalance = 0;

   if (parentBalance > 0)
   {
      if (childBalance < 0)
      {
         bAVLRotateRight(childNode);
         bAVLRotateLeft(node);
      }
      else
         bAVLRotateLeft(node);
   }
   else if (parentBalance < 0)
   {
      if (childBalance > 0)
      {
         bAVLRotateLeft(childNode);
         bAVLRotateRight(node);
      }
      else
         bAVLRotateRight(node);
   }
}

node_t *
bAVLInsert
(tree_t *tree, uintptr_t label)
{
   avl_label_t *avlLabel;
   node_t *result;
   
   assert(tree != NULL);

   avlLabel = (avl_label_t *)bAlloc(sizeof(avl_label_t));
   avlLabel->height = 0;
   avlLabel->label = label;

   result = bTreeInsert(tree, (uintptr_t)avlLabel);

   bAVLUpdateHeight(result);

   return result;
}

void
bAVLNodeDelete
(node_t *node)
{
   assert(node != NULL);
   assert(node->tree != NULL);

   if (bNodeIsLeaf(node))
   {
      node_t *parent;

      parent = node->parent;
      
      if (parent != NULL)
      {
         if (bNodeIsLeft(node))
            parent->left = NULL;
         else
            parent->right = NULL;

         node->parent = NULL;

         bAVLUpdateHeight(parent);
      }
      else
         node->tree->rootNode = NULL;
   }
   else if (!bNodeIsFull(node))
   {
      node_t *replacement;

      if (node->left == NULL)
         replacement = node->right;
      else
         replacement = node->left;

      if (bNodeIsRoot(node))
         node->tree->rootNode = replacement;
      else if (bNodeIsLeft(node))
         bNodeReRoot(replacement, node->parent, false);
      else
         bNodeReRoot(replacement, node->parent, true);

      if (node->parent != NULL)
         bAVLUpdateHeight(node->parent);
   }
   else
   {
      node_t *leftmost, *replacement, *leftmostParent;

      leftmost = node->right;

      while (leftmost->left != NULL)
         leftmost = leftmost->left;

      replacement = leftmost->right;
      leftmostParent = leftmost->parent;

      if (leftmostParent == node)
         node->right = replacement;
      else
      {
         leftmostParent->left = replacement;

         if (replacement != NULL)
            replacement->parent = leftmostParent;
      }

      leftmost->right = node->right;
      leftmost->left = node->left;
      leftmost->parent = node->parent;
      bNodeGetAVL(leftmost)->height = bNodeGetAVL(node)->height;

      if (leftmost->left != NULL)
         leftmost->left->parent = leftmost;

      if (leftmost->right != NULL)
         leftmost->right->parent = leftmost;

      if (leftmost->parent != NULL)
      {
         if (leftmost->parent->left == node)
            leftmost->parent->left = leftmost;
         else
            leftmost->parent->right = leftmost;
      }

      if (bNodeIsRoot(node))
         node->tree->rootNode = leftmost;
      else if (node->parent != NULL)
      {
         if (bNodeIsLeft(node))
            node->parent->left = leftmost;
         else
            node->parent->right = leftmost;
      }

      if (replacement != NULL)
         bAVLUpdateHeight(replacement);
      else if (leftmostParent == node)
         bAVLUpdateHeight(leftmostParent);
      else
         bAVLUpdateHeight(leftmost);
   }

   node->left = NULL;
   node->right = NULL;
   node->parent = NULL;
}

void
bAVLTreeDestroy
(node_t *node)
{
   assert(node != NULL);
   
   if (node->left != NULL)
      bAVLTreeDestroy(node->left);

   if (node->right != NULL)
      bAVLTreeDestroy(node->right);

   bFree(bNodeGetAVL(node));
   bFree(node);
}

void
bAVLTreeFree
(tree_t *tree)
{
   assert(tree != NULL);
   
   bAVLTreeDestroy(tree->rootNode);
   bFree(tree);
}
   
intptr_t
bAVLBinCmp
(uintptr_t left, uintptr_t right)
{
   avl_label_t *leftLabel, *rightLabel;

   leftLabel = (avl_label_t *)left;
   rightLabel = (avl_label_t *)right;

   return rightLabel->label - leftLabel->label;
}
