#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include <vector>

using std::vector;
using std::cout;
using std::endl;

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator(ImageTraversal * traversal, const PNG & png, const Point & start, double tolerance, bool end) {
    traversal_ = traversal;
    png_ = png;
    start_ = start;
    current_ = start_;
    tolerance_ = tolerance;
    end_ = end;
    startPixel_ = png_.getPixel(start_.x, start_.y);
    for (unsigned i = 0; i < (png_.width() * png_.height()); i++) {
      noRepeat.push_back(false);
    }

    if (checkAdd(start_)) {
      traversal_->add(start_);
      noRepeat[start_.x + start_.y * png_.width()] = true;
    }


}

ImageTraversal::Iterator::Iterator() {
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {

  Point one = Point(current_.x + 1, current_.y);
  //cout << "Point one is " << one.x << ", " << one.y << endl;
  if (checkAdd(one)) {
    //cout << "one accepted" << endl;
    traversal_->add(one);
  }
  Point two = Point(current_.x, current_.y + 1);
  //cout << "Point two is " << two.x << ", " << two.y << endl;
  if (checkAdd(two)) {
    //cout << "two accepted" << endl;
    traversal_->add(two);
  }
  Point three = Point(current_.x - 1, current_.y);
  //cout << "Point three is " << three.x << ", " << three.y << endl;
  if (checkAdd(three)) {
    //cout << "three accepted" << endl;
    traversal_->add(three);
  }
  Point four = Point(current_.x, current_.y - 1);
  //cout << "Point four is " << four.x << ", " << four.y << endl;
  if (checkAdd(four)) {
    //cout << "four accepted" << endl;
    traversal_->add(four);
  }

  if (traversal_->empty()) {
    end_ = true;
    return *this;
  }

  Point next = traversal_->pop();
  while (noRepeat[next.x + next.y * png_.width()]) {
    if (traversal_->empty()) {
      end_ = true;
      return *this;
    }
    next = traversal_->pop();
  }
  //vector<unsigned> coord{current_.x, current_.y};
  //traversed.push_back(coord);
  /*
  for (vector<unsigned> v : traversed) {
    for (unsigned i : v) {
      cout << i << " ";
    }
    cout << "" << endl;
  }
  */
  current_ = next;
  noRepeat[current_.x + current_.y * png_.width()] = true;
  return *this;
}

bool ImageTraversal::Iterator::checkAdd(Point point) {
  if (point.x >= png_.width() || point.y >= png_.height()) {
    return false;
  }
  HSLAPixel currPixel = png_.getPixel(point.x, point.y);
  /*
  for (vector<unsigned> v : traversed) {
    if (point.x == v.at(0) && point.y == v.at(1)) {
      return false;
    }
  }
  */
  // traversal_->noRepeat(point) &&
  if (calculateDelta(startPixel_, currPixel) < tolerance_) {
    //cout << "check add is ok" << endl;
    return true;
  } else {
    return false;
  }
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return current_;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return !(end_== other.end_);
}
