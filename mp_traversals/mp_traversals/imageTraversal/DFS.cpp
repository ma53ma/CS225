#include <iterator>
#include <cmath>

#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include "DFS.h"

using namespace cs225;
using std::vector;
using std::stack;
using std::cout;
using std::endl;


/**
 * Initializes a depth-first ImageTraversal on a given `png` image,
 * starting at `start`, and with a given `tolerance`.
 *
 * @param png The image this DFS is going to traverse
 * @param start The start point of this DFS
 * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
 * it will not be included in this DFS
 */
DFS::DFS(const PNG & png, const Point & start, double tolerance) {
  png_ = png;
  start_ = start;
  tolerance_ = tolerance;
  Iterator it = ImageTraversal::Iterator(this, png_, start_, tolerance_, false);
}
/*
bool DFS::noRepeat(Point current) {
  stack<Point> s2 = s;
  stack<Point> garbage;
  while (!s2.empty()) {
    Point testPoint = s2.top();
    //cout << "testing " << current.x << " and " << current.y << " against " << testPoint.x << " and " << testPoint.y << endl;
    if (current.x == testPoint.x && current.y == testPoint.y) {
      s = s2;
      s.pop();
      while (!garbage.empty()) {
        s.push(garbage.top());
        garbage.pop();
      }
      s.push(testPoint);
      //cout << "denied " << current.x << " and " << current.y << endl;
      return false;
    }
    garbage.push(s2.top());
    s2.pop();
  }
  return true;
}
*/

/**
 * Returns an iterator for the traversal starting at the first point.
 */
ImageTraversal::Iterator DFS::begin() {
  return ImageTraversal::Iterator(this, png_, start_, tolerance_, false);
}

/**
 * Returns an iterator for the traversal one past the end of the traversal.
 */
ImageTraversal::Iterator DFS::end() {
  return ImageTraversal::Iterator(this, png_, start_, tolerance_, true);
}

/**
 * Adds a Point for the traversal to visit at some point in the future.
 */
void DFS::add(const Point & point) {
  //cout << "adding " << point.x << " and " << point.y << endl;
  s.push(point);
}

/**
 * Removes and returns the current Point in the traversal.
 */
Point DFS::pop() {
  Point returnPoint = s.top();
  s.pop();
  return returnPoint;
}

/**
 * Returns the current Point in the traversal.
 */
Point DFS::peek() const {
  return s.top();
}

/**
 * Returns true if the traversal is empty.
 */
bool DFS::empty() const {
  if (s.empty()) {
    return true;
  } else {
    return false;
  }
}
