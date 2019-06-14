#include <boring/tree.h>

tree_t *
bTreeCreate
(tree_cmp_t cmp)
{
   tree_t *result;

   assert(cmp != NULL);

   result = (tree_t *)bAlloc(sizeof(tree_t));
   result->rootNode = NULL;
   result->cmp = cmp;

   return result;
}

node_t *
bNodeCreate
(tree_t *tree, uintptr_t label, node_t *left, node_t *right
 ,node_t *parent)
{
   node_t *result;

   assert(tree != NULL);

   result = (node_t *)bAlloc(sizeof(node_t));
   result->tree = tree;
   result->label = label;
   result->left = left;
   result->right = right;
   result->parent = parent;

   return result;
}

intptr_t
bNodeCmp
(node_t *right, node_t *left)
{
   tree_t *tree;
   
   assert(right != NULL);
   assert(left != NULL);
   assert(right->tree == left->tree);

   tree = right->tree;

   assert(tree != NULL);
   assert(tree->cmp != NULL);

   return tree->cmp(left->label, right->label);
}

bool
bNodeIsRoot
(node_t *node)
{
   assert(node != NULL);

   return (node == node->tree->rootNode);
}

bool
bNodeIsLeft
(node_t *node)
{
   assert(node != NULL);

   return (!bNodeIsRoot(node) && node->parent->left == node);
}

bool
bNodeIsRight
(node_t *node)
{
   assert(node != NULL);

   return (!bNodeIsRoot(node) && node->parent->right == node);
}

bool
bNodeIsLeaf
(node_t *node)
{
   assert(node != NULL);

   return (node->left == NULL && node->right == NULL);
}

bool
bNodeIsFull
(node_t *node)
{
   assert(node != NULL);

   return (node->left != NULL && node->right != NULL);
}

void
bNodeReRoot
(node_t *node, node_t *root, bool side)
{
   assert(node != NULL);
   assert(root != NULL);

   if (side == false) /* left side */
      root->left = node;
   else
      root->right = node;

   if (bNodeIsLeft(node))
      node->parent->left = NULL;
   else
      node->parent->right = NULL;

   node->parent = root;
}

node_t *
bTreeInsert
(tree_t *tree, uintptr_t label)
{
   node_t *iter;

   assert(tree != NULL);
   assert(tree->cmp != NULL);

   iter = tree->rootNode;

   if (iter == NULL)
   {
      tree->rootNode = bNodeCreate(tree, label, NULL, NULL, NULL);
      return tree->rootNode;
   }

   while (iter != NULL)
   {
      intptr_t result;

      result = tree->cmp(iter->label, label);

      if (result < 0)
      {
         if (iter->left == NULL)
         {
            iter->left = bNodeCreate(tree, label, NULL, NULL, iter);
            return iter->left;
         }
         else
            iter = iter->left;
      }
      else if (result > 0)
      {
         if (iter->right == NULL)
         {
            iter->right = bNodeCreate(tree, label, NULL, NULL, iter);
            return iter->right;
         }
         else
            iter = iter->right;
      }
      else
         return NULL;
   }

   return NULL;
}

node_t *
bTreeFind
(tree_t *tree, uintptr_t label)
{
   node_t *iter;

   assert(tree != NULL);
   assert(tree->cmp != NULL);

   iter = tree->rootNode;

   while (iter != NULL)
   {
      intptr_t result;

      result = tree->cmp(iter->label, label);

      if (result < 0 && iter->left != NULL)
         iter = iter->left;
      else if (result > 0 && iter->right != NULL)
         iter = iter->right;
      else if (result == 0)
         return iter;
      else
         return NULL;
   }

   return NULL;
}

void
bNodeDelete
(node_t *node)
{
   /* delete the node from the tree and return its replacement, if any */
   assert(node != NULL);

   /*
              6
           /     \
          4       14
        /   \   /   \
       2     5 10    20
      /             /  \
     1            17    25

       leaf node:
          set parent's left/right node null
          erase the leaf node
       node with single child:
          replace node with child
       node with two children:
          replace with right child
          move right node's left node to left node's rightmost end
   */

   if (bNodeIsLeaf(node))
   {
      if (bNodeIsRoot(node))
         node->tree->rootNode = NULL;
      else if (bNodeIsLeft(node))
         node->parent->left = NULL;
      else
         node->parent->right = NULL;
   }
   else if (!bNodeIsFull(node))
   {
      node_t *replacement;

      if (node->right != NULL)
         replacement = node->right;
      else
         replacement = node->left;
         
      bNodeReRoot(replacement, node->parent, bNodeIsRight(node));

      if (bNodeIsRoot(node))
      {
         node->tree->rootNode = replacement;
         replacement->parent = NULL;
      }
   }
   else if (bNodeIsFull(node))
   {
      node_t *leftNode, *rightNode, *rightLeft;

      leftNode = node->left;
      rightNode = node->right;
      rightLeft = rightNode->left;
      
      while (leftNode->right != NULL)
         leftNode = leftNode->right;

      if (rightLeft != NULL)
         bNodeReRoot(rightLeft, leftNode, true);

      bNodeReRoot(node->left, rightNode, false);
      
      if (bNodeIsRoot(node))
      {
         node->tree->rootNode = rightNode;
         rightNode->parent = NULL;
      }
      else
         bNodeReRoot(rightNode, node->parent, true);
   }

   bFree(node);
}

void
bTreeDestroy
(node_t *node)
{
   assert(node != NULL);
   
   if (node->left != NULL)
      bTreeDestroy(node->left);

   if (node->right != NULL)
      bTreeDestroy(node->right);

   bNodeDelete(node);
}

void
bTreeFree
(tree_t *tree)
{
   bTreeDestroy(tree->rootNode);
   bFree(tree);
}

intptr_t
bTreeBinCmp
(uintptr_t left, uintptr_t right)
{
   return right - left;
}
