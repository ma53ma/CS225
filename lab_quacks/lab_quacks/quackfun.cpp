#include <stack>
#include <iostream>

using std::cout;
using std::endl;

/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 * stacks and queues portion of the lab.
 */

namespace QuackFun {

/**
 * Sums items in a stack.
 *
 * **Hint**: think recursively!
 *
 * @note You may modify the stack as long as you restore it to its original
 * values.
 *
 * @note You may use only two local variables of type T in your function.
 * Note that this function is templatized on the stack's type, so stacks of
 * objects overloading the + operator can be summed.
 *
 * @note We are using the Standard Template Library (STL) stack in this
 * problem. Its pop function works a bit differently from the stack we
 * built. Try searching for "stl stack" to learn how to use it.
 *
 * @param s A stack holding values to sum.
 * @return  The sum of all the elements in the stack, leaving the original
 *          stack in the same state (unchanged).
 */
template <typename T>
T sum(stack<T>& s)
{
  if (s.empty()) {
    return T();
  }
  T returnVal = s.top();
  s.pop();
  T secondVal = sum(s);
  s.push(returnVal);

  return returnVal + secondVal;
}

/**
 * Checks whether the given string (stored in a queue) has balanced brackets.
 * A string will consist of square bracket characters, [, ], and other
 * characters. This function will return true if and only if the square bracket
 * characters in the given string are balanced. For this to be true, all
 * brackets must be matched up correctly, with no extra, hanging, or unmatched
 * brackets. For example, the string "[hello][]" is balanced, "[[][[]a]]" is
 * balanced, "[]]" is unbalanced, "][" is unbalanced, and "))))[cs225]" is
 * balanced.
 *
 * For this function, you may only create a single local variable of type
 * `stack<char>`! No other stack or queue local objects may be declared. Note
 * that you may still declare and use other local variables of primitive types.
 *
 * @param input The queue representation of a string to check for balanced brackets in
 * @return      Whether the input string had balanced brackets
 */
bool isBalanced(queue<char> input) {
  if (input.size() == 0) {
    return true;
  }
  if (input.size() == 1) {
    return false;
  }
  std::stack<char> charStack;
  while(!input.empty()) {
    if (input.front() ==  ']' || input.front() == '[') {
      if (charStack.size() == 0 && input.front() == ']') {
        return false;
      }
      charStack.push(input.front());
    }
    input.pop();
  }
  char * charArray = new char[charStack.size()];
  int count = 0;
  while(!charStack.empty()) {
    if (charStack.top() != ']') {
      delete[] charArray;
      return false;
    } else {
      if (charStack.size() == 1) {
        delete[] charArray;
        return false;
      }
      charStack.pop();
      while (!charStack.empty()) {
        if (charStack.top() == ']') {
          *(charArray + count) = ']';
          count++;
          charStack.pop();
        } else {
          charStack.pop();
          for (int i = 0; i < count; i++) {
            charStack.push(*(charArray + i));
          }
          count = 0;
          break;
        }
      }
    }
  }
  if (charStack.size() == 0) {
    delete[] charArray;
    return true;
  } else {
    delete[] charArray;
    return false;
  }
}

/**
 * Reverses even sized blocks of items in the queue. Blocks start at size
 * one and increase for each subsequent block.
 *
 * **Hint**: You'll want to make a local stack variable.
 *
 * @note Any "leftover" numbers should be handled as if their block was
 * complete.
 *
 * @note We are using the Standard Template Library (STL) queue in this
 * problem. Its pop function works a bit differently from the stack we
 * built. Try searching for "stl stack" to learn how to use it.
 *
 * @param q A queue of items to be scrambled
 */
template <typename T>
void scramble(queue<T>& q) {
  stack<T> s;
  stack<T> s2;
  while (!q.empty()) {
    s.push(q.front());
    q.pop();
  }
  while (!s.empty()) {
    s2.push(s.top());
    s.pop();
  }
  int * numArray = new int[s2.size()];
  int refLength = s2.size();
  int index = 0;
  int reverseNum = 2;
  *(numArray + index) = s2.top();
  index++;
  s2.pop();
  while (!s2.empty()) {
    for (int i = 0; i < reverseNum; i++) {
      if ((!s2.empty())) {
        s.push(s2.top());
        s2.pop();
      }
    }
    for (int j = 0; j < reverseNum; j++) {
      if (!s.empty() && index < refLength) {
        *(numArray + index) = s.top();
        index++;
        s.pop();
      }
    }
    for (int k = 0; k < reverseNum + 1; k++) {
      if (!s2.empty() && index < refLength) {
        *(numArray + index) = s2.top();
        index++;
        s2.pop();
      }
    }
    reverseNum = reverseNum + 2;
  }
  for (int i = 0; i < refLength; i++) {
    q.push(numArray[i]);
  }
  delete[] numArray;
}

/**
 * Checks if the parameter stack and queue are the same. A stack and a queue
 * are considered to the "the same" if they contain only elements of exactly
 * the same values in exactly the same order.
 *
 * @note You may assume the stack and queue contain the same number of items!
 *
 * @note The back of the queue corresponds to the top of the stack!
 *
 * @note There are restrictions for writing this function.
 * - Your function may not use any loops
 * - In your function you may only declare ONE local boolean variable to use in
 *   your return statement, and you may only declare TWO local variables of
 *   parametrized type T to use however you wish.
 * - No other local variables can be used.
 * - After execution of verifySame, the stack and queue must be unchanged. Be
 *   sure to comment your code VERY well.
 *
 * @param s The stack to compare
 * @param q The queue to compare
 * @return  true if the stack and the queue are the same; false otherwise
 */
template <typename T>
bool verifySame(stack<T>& s, queue<T>& q)
{
  if (s.empty() && q.empty()) {
    return true;
  } else if (s.top() != q.front()) {
    T temp2 = q.front();
  }
    return false;
}

}
