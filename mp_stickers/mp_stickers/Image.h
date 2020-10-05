#include "cs225/PNG.h"


#pragma once

using namespace cs225;

class Image : public PNG {
  public:
    Image(const Image & other);
    Image();
    Image(int width, int height);
    ~Image();
    void lighten();
    void lighten(double amount);
    void darken();
    void darken(double amount);
    void saturate();
    void saturate(double amount);
    void desaturate();
    void desaturate(double amount);
    void grayscale();
    void rotateColor(double degrees);
    void illinify();
    void scale(double factor);
    void scale(unsigned w, unsigned h);
    void scaleUp(double factor);
    void scaleDown(double factor);
    void copy_(const Image & other);
    void destroy();
    const Image & operator=(const Image &other);
    int * xStart;
    int * yStart;
};
