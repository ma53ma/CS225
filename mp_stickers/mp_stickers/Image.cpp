#include "Image.h"
#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "stdio.h"
#include "math.h"

#include <algorithm>

using cs225::HSLAPixel;
using cs225::PNG;

Image::Image(const Image & other) : PNG(other) {
  copy_(other);
}

Image::Image() {
  xStart = new int;
  yStart = new int;
  *xStart = 0;
  *yStart = 0;
}

Image::Image(int width, int height) : PNG(width, height) {
  xStart = new int;
  yStart = new int;
  *xStart = 0;
  *yStart = 0;
}

Image::~Image() {
  destroy();
}

void Image::destroy() {
  delete xStart;
  delete yStart;
}

void Image::copy_(const Image & other) {
  PNG::resize(other.width(),other.height());
  xStart = new int;
  yStart = new int;
  *xStart = 0;
  *yStart = 0;;
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & mainPixel = PNG::getPixel(x, y);
      HSLAPixel & otherPixel = other.getPixel(x,y);
      mainPixel.h = otherPixel.h;
      mainPixel.s = otherPixel.s;
      mainPixel.l = otherPixel.l;
      mainPixel.a = otherPixel.a;
    }
  }
}

const Image & Image::operator=(const Image &other) {
  if (this != &other) {
    destroy();
    copy_(other);
  }
  return *this;
}

void Image::lighten() {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      if (pixel.l <= .9) {
        pixel.l = pixel.l + .1;
      } else {
        pixel.l = 1.0;
      }
    }
  }
}

void Image::lighten(double amount) {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      if (amount >= 0) {
        if (pixel.l <= (1 - amount)) {
          pixel.l = pixel.l + amount;
        } else {
          pixel.l = 1.0;
        }
      } else {
        if (pixel.l >= (-1 * amount)) {
          pixel.l = pixel.l + amount;
        } else {
          pixel.l = 0.0;
        }
      }
    }
  }
}

void Image::darken() {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      if (pixel.l >= .1) {
        pixel.l = pixel.l - .1;
      } else {
        pixel.l = 0.0;
      }
    }
  }
}
void Image::darken(double amount) {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      if (amount >= 0) {
        if (pixel.l >= amount) {
          pixel.l = pixel.l - amount;
        } else {
          pixel.l = 0.0;
        }
      } else {
        if (pixel.l <= (1 - (-1 * amount))) {
          pixel.l = pixel.l - amount;
        } else {
          pixel.l = 1.0;
        }
      }
    }
  }
}

void Image::saturate() {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      if (pixel.s <= .9) {
        pixel.s = pixel.s + .1;
      } else {
        pixel.s = 1.0;
      }
    }
  }
}

void Image::saturate(double amount) {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      if (amount >= 0) {
        if (pixel.s <= (1 - amount)) {
          pixel.s = pixel.s + amount;
        } else {
          pixel.s = 1.0;
        }
      } else {
        if (pixel.s >= (-1 * amount)) {
          pixel.s = pixel.s + amount;
        } else {
          pixel.s = 0.0;
        }
      }
    }
  }
}

void Image::desaturate() {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = getPixel(x, y);
      if (pixel.s >= .1) {
        pixel.s = pixel.s - .1;
      } else {
        pixel.s = 0.0;
      }
    }
  }
}

void Image::desaturate(double amount) {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      if (amount >= 0) {
        if (pixel.s >= amount) {
          pixel.s = pixel.s - amount;
        } else {
          pixel.s = 0.0;
        }
      } else {
        if (pixel.s <= (1 - (-1 * amount))) {
          pixel.s = pixel.s - amount;
        } else {
          pixel.s = 1.0;
        }
      }
    }
  }
}

void Image::grayscale() {
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      pixel.s = 0.0;
    }
  }
}

void Image::rotateColor(double degrees) {
  if (degrees >= 0) {
    for (unsigned x = 0; x < PNG::width(); x++) {
      for (unsigned y = 0; y < PNG::height(); y++) {
        HSLAPixel & pixel = PNG::getPixel(x,y);
        if (pixel.h <= (360-degrees)) {
          pixel.h = pixel.h + degrees;
        } else {
          pixel.h = pixel.h + degrees - 360;
        }
      }
    }
  } else {
    for (unsigned x = 0; x < PNG::width(); x++) {
      for (unsigned y = 0; y < PNG::height(); y++) {
        HSLAPixel & pixel = PNG::getPixel(x,y);
        if (pixel.h >= (-1 * degrees)) {
          pixel.h = pixel.h + degrees;
        } else {
          pixel.h = 360 + pixel.h + degrees;
        }
      }
    }
  }
}

void Image::illinify() {
  for (unsigned x = 0; x < PNG::width(); x++) {
    for (unsigned y = 0; y < PNG::height(); y++) {
      HSLAPixel & pixel = PNG::getPixel(x, y);
      if (113.5 < pixel.h && pixel.h < 293.5) {
        pixel.h = 216;
      } else {
        pixel.h = 11;
      }
    }
  }
}
void Image::scale(double factor) {
  if (factor >= 1) {
    scaleUp(factor);
  } else {
    scaleDown(factor);
  }
}

void Image::scaleDown(double factor) {
  unsigned int xCount = 0;
  unsigned int yCount = 0;
  PNG * scaledImage = new PNG (PNG::width()*factor, PNG::height()*factor);
  for (unsigned int x = 0; x < PNG::width(); x += (1/factor)) {
    yCount = 0;
    for (unsigned int y = 0; y < PNG::height(); y += (1/factor)) {
      if (yCount >= scaledImage->height()) {
        break;
      }
      HSLAPixel & ogPixel = PNG::getPixel(x,y);
      HSLAPixel & newPixel = scaledImage->getPixel(xCount, yCount);
      newPixel.h = ogPixel.h;
      newPixel.s = ogPixel.s;
      newPixel.l = ogPixel.l;
      newPixel.a = ogPixel.a;
      yCount++;
    }
    yCount = 0;
    xCount++;
    if (xCount >= scaledImage->width()) {
      break;
    }
  }
  PNG::resize(PNG::width()*factor,PNG::height()*factor);
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & scaledPixel = scaledImage->getPixel(x,y);
      HSLAPixel & ogPixel = PNG::getPixel(x,y);
      ogPixel.h = scaledPixel.h;
      ogPixel.s = scaledPixel.s;
      ogPixel.l = scaledPixel.l;
      ogPixel.a = scaledPixel.a;
    }
  }
  delete scaledImage;
}

void Image::scaleUp(double factor) {
  int xCount = 0;
  int yCount = 0;
  PNG * scaledImage = new PNG (PNG::width()*factor, PNG::height()*factor);
  for (unsigned int y = 0; y < PNG::height(); y++) {
    for (unsigned int x = 0; x < PNG::width(); x++) {
      HSLAPixel & ogPixel = PNG::getPixel(x,y);
      for (unsigned int i = 0; i < factor; i++) {
        for (unsigned int j = 0; j < factor; j++) {
          HSLAPixel & newPixel = scaledImage->getPixel(x + i + xCount, y + yCount + j);
          newPixel.h = ogPixel.h;
          newPixel.s = ogPixel.s;
          newPixel.l = ogPixel.l;
          newPixel.a = ogPixel.a;
        }
      }
      xCount += (factor - 1);
    }
    xCount = 0;
    yCount += (factor - 1);
  }
  PNG::resize(PNG::width()*factor,PNG::height()*factor);
  for (unsigned int x = 0; x < PNG::width(); x++) {
    for (unsigned int y = 0; y < PNG::height(); y++) {
      HSLAPixel & scaledPixel = scaledImage->getPixel(x,y);
      HSLAPixel & ogPixel = PNG::getPixel(x,y);
      ogPixel.h = scaledPixel.h;
      ogPixel.s = scaledPixel.s;
      ogPixel.l = scaledPixel.l;
      ogPixel.a = scaledPixel.a;
    }
  }
  delete scaledImage;
}

void Image::scale(unsigned w, unsigned h) {
  double ratioW = (double) w / (double) PNG::width();
  double ratioH = (double) h / (double) PNG::height();
  double ratio = std::min(ratioW, ratioH);
  if (ratio < 1) {
    scaleDown(ratio);
  } else {
    scaleUp(ratio);
  }
}
