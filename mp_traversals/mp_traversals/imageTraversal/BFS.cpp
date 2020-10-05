#include <iterator>
#include <cmath>

#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include "BFS.h"

using namespace cs225;
using std::vector;
using std::stack;
using std::endl;
using std::cout;

/**
 * Initializes a breadth-first ImageTraversal on a given `png` image,
 * starting at `start`, and with a given `tolerance`.
 * @param png The image this BFS is going to traverse
 * @param start The start point of this BFS
 * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
 * it will not be included in this BFS
 */
BFS::BFS(const PNG & png, const Point & start, double tolerance) {
  png_ = png;
  start_ = start;
  tolerance_ = tolerance;
  Iterator it = ImageTraversal::Iterator(this, png_, start_, tolerance_, false);
}
/*
bool BFS::noRepeat(Point current) {
  queue<Point> q2 = q;
  while (!q2.empty()) {
    Point testPoint = q2.front();
    //cout << current.x << " and " << current.y << " and " << testPoint.x << " and " << testPoint.y << " and " << endl;
    if (current.x == testPoint.x && current.y == testPoint.y) {
      //cout << "denied" << endl;
      return false;
    }
    q2.pop();
  }
  return true;
}
*/
/**
 * Returns an iterator for the traversal starting at the first point.
 */
ImageTraversal::Iterator BFS::begin() {
  return ImageTraversal::Iterator(this, png_, start_, tolerance_, false);
}

/**
 * Returns an iterator for the traversal one past the end of the traversal.
 */
ImageTraversal::Iterator BFS::end() {
  return ImageTraversal::Iterator(this, png_, start_, tolerance_, true);
}

/**
 * Adds a Point for the traversal to v
 isit at some point in the future.
 */
void BFS::add(const Point & point) {
  //cout << point.x << " and " << point.y << endl;
  q.push(point);
}

/**
 * Removes and returns the current Point in the traversal.
 */
Point BFS::pop() {
  Point returnPoint = q.front();
  q.pop();
  return returnPoint;
}

/**
 * Returns the current Point in the traversal.
 */
Point BFS::peek() const {
  return q.front();
}

/**
 * Returns true if the traversal is empty.
 */
bool BFS::empty() const {
  if (q.empty()) {
    return true;
  } else {
    return false;
  }
}
