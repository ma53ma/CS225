#include "../cs225/HSLAPixel.h"
#include "../Point.h"
#include <stdlib.h>

#include "ColorPicker.h"
#include "MyColorPicker.h"

using namespace cs225;

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */
HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y) {
  double hue = (double) (rand()) / (double) (RAND_MAX / 360.0);
  double sat = (double) (rand()) / (double) (RAND_MAX);
  HSLAPixel pixel(hue, sat, .5, 1.0);
  return pixel;
}
