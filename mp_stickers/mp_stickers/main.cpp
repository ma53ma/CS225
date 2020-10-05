#include "Image.h"
#include "StickerSheet.h"

#include <iostream>
int main() {
  Image spider1; spider1.readFromFile("tests/spider1.png");
  Image base;    base.readFromFile("tests/red blue.png");

  StickerSheet sheet(base, 3);

  spider1.scale(.1);

  sheet.addSticker(spider1,400, 400);

  PNG final = sheet.render();
  final.writeToFile("renderTest.png");


  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //

  return 0;
}
