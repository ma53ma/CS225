/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

using std::cout;
using std::endl;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    if (first[curDim] < second[curDim]) {
      return true;
    } else if (first[curDim] > second[curDim]) {
      return false;
    } else {
      return (first < second);
    }
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    if (eucDist(target, potential) < eucDist(currentBest, target)) {
      return true;
    } else if (eucDist(target, potential) > eucDist(currentBest, target)) {
      return false;
    } else {
      return (potential < currentBest);
    }
}

template <int Dim>
double KDTree<Dim>::eucDist(Point<Dim> first, Point<Dim> second) const {
  double sum = 0;
  for (int i = 0; i < Dim; i++) {
    sum += pow(first[i] - second[i], 2);
  }
  return sum;
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& newPoints, int left, int right, int pivotIndex, int dim) { //pivotIndex is the index you are splitting around
  Point<Dim> pivotVal = newPoints[pivotIndex]; //value you are splitting around
  std::swap(newPoints[pivotIndex], newPoints[right]); //move value to end
  int storeIndex = left;
  for (int i = left; i < right; i++) {
    if (smallerDimVal(newPoints[i], pivotVal, dim)) { //if value is less than pivot val, put it to the left
      std::swap(newPoints[storeIndex], newPoints[i]);
      storeIndex++;
    }
  }
  std::swap(newPoints[right], newPoints[storeIndex]); //put pivot val in correct place
  return storeIndex; //place where pivot value is
}

template <int Dim>
Point<Dim> KDTree<Dim>::select(vector<Point<Dim>>& newPoints, int left, int right, int k, int dim) { //returns the kth smallest elem
  if (left == right) {
    return newPoints[left];
  }
  int pivotIndex = ((left + right) / 2);
  pivotIndex = partition(newPoints, left, right, pivotIndex, dim);
  if (k == pivotIndex) { //if our desired val is in the pivot pos, return
    return newPoints[k];
  } else if (k < pivotIndex) { //if desired val is less than pivot, recurse on smaller half
    return select(newPoints, left, pivotIndex - 1, k, dim);
  } else { //otherwise, recurse on bigger half
    return select(newPoints, pivotIndex + 1, right, k, dim);
  }
}


template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    // will pass in vector size / 2 floor for k for select
    /**
     * @todo Implement this function!
     */
     realPoints = newPoints;

     root = KDTreeHelper(realPoints, 0, realPoints.size() - 1, 0);
}

template<int Dim>
typename KDTree<Dim>::KDTreeNode * KDTree<Dim>::KDTreeHelper(vector<Point<Dim>>& realPoints, int left, int right, int dim) {
  int mid = (left + right) / 2;

  if (left == right) {
    return new KDTreeNode(realPoints[left]);
  }

  if (left > right) {
    return NULL;
  }

  KDTreeNode * newNode = new KDTreeNode();

  newNode->point = select(realPoints, left, right, mid, dim);

  if (left <=  mid - 1) {
    newNode->left = KDTreeHelper(realPoints, left, mid - 1, (dim + 1) % Dim);
  }
  if (right >= mid + 1) {
    newNode->right = KDTreeHelper(realPoints, mid + 1, right, (dim + 1) % Dim);
  }

  return newNode;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
   clear(other);

}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  if (&rhs == this) {
    return *this;
  }
  clear();
  copy(rhs);
  return *this;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode * KDTree<Dim>::copy(KDTree<Dim>& subtree) {
  root = copy(subtree.root);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode * KDTree<Dim>::copy(KDTreeNode * subroot) {
  if (subroot == NULL) {
    return subroot;
  } else {
    root->point = subroot->point;
    root->left = copy(subroot->left);
    root->right = copy(subroot->right);
    return root;
  }
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  clear();

}

template <int Dim>
void KDTree<Dim>::clear() {
  clear(root);
  root = NULL;
}

template <int Dim>
void KDTree<Dim>::clear(KDTreeNode * root) {
  if (root == NULL) {
    return;
  } else {
    clear(root->left);
    clear(root->right);
    delete root;
  }
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
     //root, query, dim
     if (root == NULL) {
       return Point<Dim>();
     }
     KDTreeNode * final = fNNHelper(root, query, 0);
     return final->point;

}

template <int Dim>
typename KDTree<Dim>::KDTreeNode * KDTree<Dim>::fNNHelper(typename KDTree<Dim>::KDTreeNode * root, const Point<Dim>& query, int dim) const {
  KDTreeNode * subtree = NULL;
  KDTreeNode * notSubtree = NULL;
  if (smallerDimVal(query, root->point, dim)) {
    subtree = root->left;
    notSubtree = root->right;
  } else {
    subtree = root->right;
    notSubtree = root->left;
  }

  KDTreeNode * currentBest = NULL;
  if (subtree == NULL) {
    currentBest = root;
    if (notSubtree != NULL) {
      KDTreeNode * potBest = fNNHelper(notSubtree, query, (dim + 1) % Dim);
      if (shouldReplace(query, currentBest->point, potBest->point)) {
        currentBest = potBest;
      }
    }
  } else {
    currentBest = fNNHelper(subtree, query, (dim + 1) % Dim);
    if (shouldReplace(query, currentBest->point, root->point)) {
      currentBest = root;
    }
    int currentDist = eucDist(currentBest->point, query);

    int vertDist = query[dim] - root->point[dim];
    if (pow(vertDist,2) <= currentDist) {
      if (notSubtree != NULL) {
        KDTreeNode * potBest = fNNHelper(notSubtree, query, (dim + 1) % Dim);
        if (shouldReplace(query, currentBest->point, potBest->point)) {
          currentBest = potBest;
        }
      }
    }
  }
  return currentBest;

}
