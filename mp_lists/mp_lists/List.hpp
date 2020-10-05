#include <iostream>
#include <string>
#include <math.h>

using std::cout;
using std::endl;

/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() {
  // @TODO: graded in MP3.1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {

  // @TODO: graded in MP3.1
  return List<T>::ListIterator(tail_->next);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  ListNode * current = head_;
  ListNode * hold;
  while (current != NULL) {
    hold = current->next;
    delete current;
    current = hold;
  }
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  ListNode * newNode = new ListNode(ndata);
  if (head_ != NULL) { // head_ not initialized?
    newNode -> next = head_;
    newNode -> prev = NULL;
    head_ -> prev = newNode;
    head_ = newNode;
    length_++;
  } else {
    head_ = newNode;
    tail_ = newNode;
    newNode->prev = NULL;
    newNode->next = NULL;
    length_++;
  }

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  ListNode * newNode = new ListNode(ndata);
  if (tail_ != NULL) {
    newNode->prev = tail_;
    tail_->next = newNode;
    tail_ = newNode;
  }

  if (head_ == NULL) {
    head_ = newNode;
  }

  if (tail_ == NULL) {
    tail_ = newNode;
  }

  length_++;
  /// @todo Graded in MP3.1
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.1
  if (start == NULL) {
    return NULL;
  }
  ListNode * curr = start;
  for (int i = 0; i < splitPoint; i++) {
    if (curr != NULL) {
      curr = curr->next;
    }
  }
  if (curr != NULL && curr->prev != NULL) {
    curr->prev->next = NULL;
    curr->prev = NULL;
  }
  return curr;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  if (length_ == 0 || length_ == 1) {
    return;
  }
  ListNode * current = head_->next;
  ListNode * nextNode = current->next;
  while (current != tail_ && current != NULL) {
    //remove the node
    current->prev->next = current->next;
    current->next->prev = current->prev;
    //add it to end
    current->prev = tail_;
    tail_->next = current;
    current->next = NULL;
    //move pointers
    tail_ = current;
    if (nextNode->next != NULL) {
      current = nextNode->next;
    }
    if (nextNode->next->next != NULL) {
      nextNode = nextNode->next->next;
    } else {
      return;
    }
  }
  return;
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  ListNode * first = startPoint;
  ListNode* tmp1 = first;
  ListNode * last = endPoint;
  ListNode * end = endPoint->next;
  ListNode * before = startPoint->prev;
  ListNode * tmp = startPoint;

  while (first != end) {
    tmp1 = first->prev;
    first->prev = first->next;
    first->next = tmp1;
    first = first->prev;
  }

  if (before != NULL) { //at the end
    endPoint->prev = startPoint->next;
    startPoint->next->next = endPoint;
  } else {
    endPoint->prev = NULL;
  }

  if (end != NULL) {
    startPoint->next = end;
    end->prev = startPoint;
  } else {
    startPoint->next = NULL;
  }

  ListNode * finaltemp = startPoint;
  startPoint = endPoint;
  endPoint = finaltemp;
  end = NULL;
  before = NULL;
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  if (head_ == NULL) {
    return;
  }
  double num2 = (double) size() / n;
  int num = ceil(num2);
  int count = 0;
  ListNode * start = head_;
  ListNode * end = start;
  bool fix = true;
  while (end->next != NULL && start->next != NULL) {
    count++;
    end = start;
    for (int i = 0; i < n - 1; i++) {
      if (end->next != NULL) {
        end = end->next;
      }
    }
    reverse(start, end);
    if (fix) {
      head_ = start;
      fix = false;
    }
    if (count == num) {
      tail_ = end;
    }
    start = end->next;
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  ListNode * newHead = NULL;
  ListNode * current = newHead;
  while (true) {
    if (first == NULL) {
      current->next = second;
      break;
    } else if (second == NULL) {
      current->next = first;
      break;
    }
    if (first->data < second->data) {
      if (newHead == NULL) {
        newHead = first;
        first->prev = NULL;
        current = first;
        first = first->next;
        if (current->next != NULL) {
          current->next->prev = NULL;
          current->next = NULL;
        }
      } else {
        current->next = first;
        first->prev = current;
        current = first;
        first = first->next;
        if (current->next != NULL) {
          current->next->prev = NULL;
          current->next = NULL;
        }
      }
    } else {
      if (newHead == NULL) {
        newHead = second;
        second->prev = NULL;
        current = second;
        second = second->next;
        if (current->next != NULL) {
          current->next->prev = NULL;
          current->next = NULL;
        }
      } else {
        current->next = second;
        second->prev = current;
        current = second;
        second = second->next;
        if (current->next != NULL) {
          current->next->prev = NULL;
          current->next = NULL;
        }
      }
    }
  }
  return newHead;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  if (chainLength == 1) {
    return start;
  }
  ListNode * splitList = split(start, chainLength/2);
  return merge(mergesort(splitList, ceil((double) chainLength/2)), mergesort(start, chainLength/2));
}

template <typename T>
typename List<T>::ListNode *& List<T>::_index_helper(unsigned index, List<T>::ListNode *& node) {
  if (index == 0) {
    return node;
  }
  return _index_helper(index - 1, node->next);
}


template <typename T>
const T & List<T>::remove(unsigned index) {
  ListNode *& node = _index(index);
  ListNode * temp  = node;
  const T & returnData = node -> data;
  (node->next)->prev = node;
  node = node -> next;
  delete temp;
  return returnData;
}

template <typename T>
typename List<T>::ListNode *& List<T>::_index(unsigned index) {
  return _index_helper(index, head_);
}
