#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

#include <string>
#include <cmath>

using namespace cs225;


void rotate(std::string inputFile, std::string outputFile) {
  PNG *inputImage = new PNG();
  inputImage->readFromFile(inputFile);
  int inputWidth = inputImage->width();
  int inputHeight = inputImage->height();

  PNG *outputImage = new PNG(inputWidth, inputHeight);


  for (int x = 0; x < inputWidth; x++) {
    for (int y = 0; y < inputHeight; y++) {
      HSLAPixel & inputPixel = inputImage->getPixel(x, y);
      HSLAPixel & outputPixel = outputImage->getPixel(inputWidth - 1 - x, inputHeight -1 - y);
      outputPixel = inputPixel;
    }
  }
  outputImage->writeToFile(outputFile);
  delete inputImage;
  delete outputImage;
}

cs225::PNG myArt( unsigned int width, unsigned int height) {
  cs225::PNG png(width, height);
  int counterOG = 0;
  int counter = counterOG;
  for (unsigned int x = 0; x < png.width(); x++) {
    counter = counterOG;
    for (unsigned int y = 0; y < png.height(); y++) {
      HSLAPixel & pixel = png.getPixel(x, y);
      if (counter >= 0 && counter < 10) {
        pixel.h = 284;
        pixel.s = 1;
        pixel.l = .08;
        pixel.a = 1;
        counter++;
      } else if (10 <= counter && counter < 20) {
        double eucDistX = abs(50 - x);
        double eucDistY = abs(1000 - y);
        double eucD = sqrt(pow(eucDistX,2) + pow(eucDistY,2));
        if (eucD < 1000) {
          pixel.h = 279;
          pixel.s = .25;
          pixel.l = .62;
          pixel.a = 1;
          counter++;
        } else if (eucD >= 1000 && eucD < 1100) {
          pixel.h = 279;
          pixel.s = .25;
          pixel.l = .62 - (eucD/2000)*.47;
          pixel.a = 1;
          counter++;
        } else {
          pixel.h = 279;
          pixel.s = .25;
          pixel.l = .05;
          pixel.a = 1;
          counter++;
        }
      } else {
        pixel.h = 284;
        pixel.s = 1;
        pixel.l = .08;
        pixel.a = 1;
        counter = 1;
      }
      if (y == png.height() - 1) {
        counterOG--;
        if (counterOG < 0) {
          counterOG = 19;
        }
      }
    }
  }
  int red_start = 0;
  for (unsigned int x = 0; x < png.width(); x++) {
    for (unsigned int y = 0; y < png.height() / 1.8; y++) {
      if (y == red_start) {
        for (unsigned int j = 0; j < 3; j++) {
          for (unsigned int i = 0; i < 5; i++) {
            for (unsigned int a = 0; a < 10; a++) {
              HSLAPixel & pixel = png.getPixel(x + j + a, y + i);
              pixel.h = 0;
              pixel.s = 1;
              pixel.l = .49;
              pixel.a = 1;
            }
          }
        }
        x += 2;
      }
    }
    red_start = red_start + 4;
  }
  for (unsigned int x = 0; x < png.width(); x++) {
    for (unsigned int y = 0; y < png.height(); y++) {
      HSLAPixel & pixel = png.getPixel(x, y);
      double eucDistX = abs(png.width()/2.1 - x);
      double eucDistY = abs(5*png.height()/8 - y);
      double eucD = sqrt(pow(eucDistX,2) + pow(eucDistY,2));
      double shadeX = abs(330 - x);
      double shadeY = abs(440 - y);
      double shadeD = sqrt(pow(shadeX,2) + pow(shadeY,2));
      if (eucD < png.width()/8) {
        if (shadeD < 120) {
          pixel.h = 235;
          pixel.s = .06;
          pixel.l = .52;
          pixel.a = 1;
        } else {
          pixel.h = 235;
          pixel.s = .06;
          pixel.l = .30;
          pixel.a = 1;
        }
      } 
    }
  }
  for (unsigned int x = 0; x < png.width(); x++) {
    for (unsigned int y = 0; y < png.height(); y++) {
      HSLAPixel & pixel = png.getPixel(x, y);
      double eucDistX = abs(330 - x);
      double eucDistY = abs(450 - y);// 20 y and 15 x
      double eucDistX2 = abs(342 - x);
      double eucDistY2 = abs(433 - y);
      double eucD = sqrt(pow(eucDistX,2) + pow(eucDistY,2));
      double eucD2 = sqrt(pow(eucDistX2,2) + pow(eucDistY2,2));
      if (eucD < 15 || eucD2 < 10) {
        pixel.h = 279;
        pixel.s = .25;
        pixel.l = .88;
        pixel.a = 1;
      }
    }
  }
  return png;
}
