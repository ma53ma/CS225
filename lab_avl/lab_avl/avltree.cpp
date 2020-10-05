/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

 #include <cmath>

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    Node * temp = t->right;
    t->right = temp->left;
    temp->left = t;
    t->height = std::fmax(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;
    t = temp;
    t->height = std::fmax(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;

}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    Node * temp = t->left;
    t->left = temp->right;
    temp->right = t;
    t->height = std::fmax(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;
    t = temp;
    t->height = std::fmax(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;

}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft");
    rotateRight(t->right); // Stores the rotation name (don't remove this)
    rotateLeft(t);// your code here
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
  if (balance(subtree) == -2) {
    if (balance(subtree->left) == -1) {
      rotateRight(subtree);
    } else {
      rotateLeftRight(subtree);
    }
  } else if (balance(subtree) == 2 ) {
    if (balance(subtree->right) == 1) {
      rotateLeft(subtree);
    } else {
      rotateRightLeft(subtree);
    }
  }
  subtree->height = std::fmax(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right)) + 1;
}

template <class K, class V>
int AVLTree<K ,V>::balance(Node * subtree) {
  return heightOrNeg1(subtree->right) - heightOrNeg1(subtree->left);
}

template <class K, class V>
int AVLTree<K, V>::height(Node * node) {
  if (node == NULL) {
    return -1;
  }
  if (node->left == NULL && node->right == NULL) {
    return 0;
  }
  if (node->left != NULL && node->right != NULL) {
    return 1 + std::fmax(height(node->left), height(node->right));
  }
  if (root->left != NULL) {
    return 1 + height(root->left);
  } else {
    return 1 + height(root->right);
  }

}
template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
  if (subtree == NULL) {
    subtree = new Node(key, value);
    return;
  }
  if (subtree->key > key) {
    insert(subtree->left, key, value);
    rebalance(subtree);
  } else {
    insert(subtree->right, key, value);
    rebalance(subtree);
  }
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        remove(subtree->right, key);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            delete subtree;
            subtree = NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            Node * temp = subtree->left;
            while (temp->right != NULL) {
              temp = temp->right;
            }
            swap(subtree,temp);
            remove(subtree->left, key);
            // your code here
        } else {
            Node * lefty = subtree->left;
            Node * righty = subtree->right;
            delete subtree;
            if (subtree->left != NULL) {
              subtree = lefty;
            } else {
              subtree = righty;
            }
        }
        // your code here
    }
    if (subtree != NULL) {
      rebalance(subtree);
    }
}
