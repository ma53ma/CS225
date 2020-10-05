/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include "TreeTraversals/InorderTraversal.h"
#include <iostream>
#include <vector>

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + std::max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    std::cout << std::endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    std::cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
  mirror(root);
}

template <typename T>
void BinaryTree<T>::mirror(Node* subRoot) const {
  if (subRoot != NULL) {
    Node * tmp = subRoot->left;
    subRoot->left = subRoot->right;
    subRoot->right = tmp;
    mirror(subRoot->left);
    mirror(subRoot->right);
  }
}



/**
 * isOrdered() function iterative version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedIterative() const
{
  InorderTraversal<int> iot(this->getRoot());
  int past = 0;
  for (TreeTraversal<int>::Iterator it = iot.begin(); it != iot.end(); ++it) {
    if ((*it)->elem < past) {
      return false;
    }
    past = (*it)->elem;
  }
  return true;
}

/**
 * isOrdered() function recursive version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedRecursive() const
{
  return recursiveHelper(getRoot());
}

template <typename T>
bool BinaryTree<T>::recursiveHelper(Node * root) const {
  bool ordered = true;

  if (root == NULL) {
    return true;
  }

  if (root->left != NULL) {
    ordered = (root->elem >= leftMax(root->left));
  }

  if (root->right != NULL) {
    ordered = (root->elem <= rightMin(root->right));
  }

  return ordered && recursiveHelper(root->left) && recursiveHelper(root->right);
}

template <typename T>
T BinaryTree<T>::leftMax(Node * subroot) const {
  if (subroot->left == NULL && subroot->right == NULL) {
    return subroot->elem;
  } else if (subroot->right == NULL) {
    return std::max(subroot->elem, leftMax(subroot->left));
  } else if (subroot->left == NULL) {
    return std::max(subroot->elem, leftMax(subroot->right));
  } else {
    return (std::max(subroot->elem, std::max(leftMax(subroot->right), leftMax(subroot->left))));
  }
}

template <typename T>
T BinaryTree<T>::rightMin(Node * subroot) const {
  if (subroot->left == NULL && subroot->right == NULL) {
    return subroot->elem;
  } else if (subroot->right == NULL) {
    return std::min(subroot->elem, rightMin(subroot->left));
  } else if (subroot->left == NULL) {
    return std::min(subroot->elem, rightMin(subroot->right));
  } else {
    return (std::min(subroot->elem, std::min(rightMin(subroot->right), rightMin(subroot->left))));
  }
}


/**
 * creates vectors of all the possible paths from the root of the tree to any leaf
 * node and adds it to another vector.
 * Path is, all sequences starting at the root node and continuing
 * downwards, ending at a leaf node. Paths ending in a left node should be
 * added before paths ending in a node further to the right.
 * @param paths vector of vectors that contains path of nodes
 */
template <typename T>
void BinaryTree<T>::getPaths(std::vector<std::vector<T> > & paths) const
{
  std::vector<int> intVect;
  addToVector(root, paths, intVect);
}

template <typename T>
void BinaryTree<T>::addToVector(Node * node, std::vector<std::vector<T> > & pathsVect, std::vector<int> intVect) const {
  if (node == NULL) {
    return;
  }
  intVect.push_back(node->elem);
  if (node->left == NULL && node->right == NULL) {
    pathsVect.push_back(intVect);
  } else {
    addToVector(node->left, pathsVect, intVect);
    addToVector(node->right,pathsVect, intVect);
  }
}


/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
    return distRecursive(root, 0);
}

template <typename T>
int BinaryTree<T>::distRecursive(Node * subroot, int layer) const {
  if (subroot == NULL) {
    return 0;
  }
  return layer + distRecursive(subroot->left, layer + 1) + distRecursive(subroot->right, layer + 1);
}
