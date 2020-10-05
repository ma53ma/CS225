#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
/**
 * @file heap.cpp
 * Implementation of a heap class.
 */

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    // @TODO Update to return the index you are choosing to be your root.
    return 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the left child.
    return 2 * currentIdx;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the right child.
    return (2 * currentIdx) + 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    // @TODO Update to return the index of the parent.
    return currentIdx / 2;
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    // @TODO Update to return whether the given node has a child
    return (leftChild(currentIdx) < _elems.size());
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the child with highest priority
    ///   as defined by higherPriority()
    size_t left = leftChild(currentIdx) ; size_t right = rightChild(currentIdx);
    if (right >= _elems.size()) {
      return left;
    }
    return higherPriority(_elems[left], _elems[right]) ? left : right;
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    // @TODO Implement the heapifyDown algorithm.
    if (hasAChild(currentIdx)) {
      size_t minChildIndex = maxPriorityChild(currentIdx);
      if (higherPriority(_elems[minChildIndex], _elems[currentIdx])) {
        std::swap(_elems[minChildIndex], _elems[currentIdx]);
        heapifyDown(minChildIndex);
      }
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    if (currentIdx != root()) {
      size_t parentIdx = parent(currentIdx);
      if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
          std::swap(_elems[currentIdx], _elems[parentIdx]);
          heapifyUp(parentIdx);
      }
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{
    // @TODO Depending on your implementation, this function may or may
    ///   not need modifying
    _elems.push_back(T());
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems)
{
    // @TODO Construct a heap using the buildHeap algorithm
    _elems.push_back(T());
    _elems.insert(_elems.end(), elems.begin(), elems.end());
    for (unsigned i = parent(_elems.size()); i > 0; i--) {
      heapifyDown(i);
    }
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    // @TODO Remove, and return, the element with highest priority
    if(empty()) {
      return T();
    }
    T minVal = _elems[root()];
    _elems[root()] = _elems[_elems.size() - 1];
    _elems.pop_back();
    heapifyDown(root());
    return minVal;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    // @TODO Return, but do not remove, the element with highest priority
    return _elems[root()];
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    // @TODO Add elem to the heap
    if (_elems.empty()) {
      _elems.push_back(T());
    }
    _elems.push_back(elem);
        //cout << "pushing: " << _elems[_elems.size() - 1] << endl;
    heapifyUp(_elems.size() - 1);
}

template <class T, class Compare>
void heap<T, Compare>::updateElem(const size_t & idx, const T& elem)
{
    // @TODO In-place updates the value stored in the heap array at idx
    // Corrects the heap to remain as a valid heap even after update
    _elems[idx] = elem;
    heapifyUp(idx);
    heapifyDown(idx);

}


template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    // @TODO Determine if the heap is empty
    return _elems.empty();
}

template <class T, class Compare>
void heap<T, Compare>::getElems(std::vector<T> & heaped) const
{
    for (size_t i = root(); i < _elems.size(); i++) {
        heaped.push_back(_elems[i]);
    }
}
