#include "StickerSheet.h"
#include "Image.h"

StickerSheet::StickerSheet(const Image& picture, unsigned max) {
  max_ = max;
  baseImage_ = new Image::Image(picture);
  refImage_ = new Image::Image(picture);
  imageArray_ = new Image[max_];
  indexArray_ = new int[max_];
  for (unsigned i = 0; i < max_; i++) {
    *(indexArray_ + i) = 0;
    *(imageArray_ + i) = picture;
  }
}

StickerSheet::StickerSheet(const StickerSheet &other) {
  _copy(other);
}

StickerSheet::~StickerSheet() {
  _destroy();
}

const StickerSheet & StickerSheet::operator=(const StickerSheet &other) {
  if (this != &other) {
    _destroy();
    _copy(other);
  }
  return *this;
}

void StickerSheet::changeMaxStickers (unsigned max) {
  Image * newImageArray = new Image::Image[max];
  int * newIndexArray = new int[max];
  for (unsigned i = 0; i < max; i++) {
    *(newIndexArray + i) = 0;
  }
  std::cout << "reached" << std::endl;
  if (max <= max_) {
    for (unsigned i = 0; i < max; i ++) {
      *(newImageArray + i) = *(imageArray_ + i);
      *(newIndexArray + i) = *(indexArray_ + i);
    }
    *baseImage_ = *refImage_;
    for (unsigned i = 0; i < max; i++) {
      addSticker(*(imageArray_ + i), *(imageArray_ + i)->xStart, *(imageArray_ + i)->yStart);
    }
  } else {
    for (unsigned i = 0; i < max_; i++) {
      *(newImageArray + i) = *(imageArray_ + i);
      *(newIndexArray + i) = *(indexArray_ + i);
    }
  }
  delete[] imageArray_;
  delete[] indexArray_;
  imageArray_ = new Image::Image[max];
  indexArray_ = new int[max];
  for (unsigned i = 0; i < max; i++) {
    *(newIndexArray + i) = 0;
    *(imageArray_ + i) = *(newImageArray + i);
    *(indexArray_ + i) = *(newIndexArray + i);
  }

  delete[] newImageArray;
  delete[] newIndexArray;
}

int StickerSheet::addSticker (Image &sticker, unsigned x, unsigned y) {
  for (unsigned i = 0; i < max_; i++) {
    if (*(indexArray_ + i) == 0) {
      *(indexArray_ + i) = i + 1;
      *(imageArray_ + i) = sticker;
      *(imageArray_ + i)->xStart = x;
      *(imageArray_ + i)->yStart = y;
      for (unsigned i = 0; x + i < (*baseImage_ ).width(); i++) {
        for (unsigned j = 0; y + j < (*baseImage_).height(); j++) {
          HSLAPixel & basePixel = (*baseImage_).getPixel(x + i, y + j);
          if (i < sticker.width() && j < sticker.height()) {
            HSLAPixel & stickerPixel = sticker.getPixel(i,j);
            if (stickerPixel.a > 0) {
              basePixel.h = stickerPixel.h;
              basePixel.s = stickerPixel.s;
              basePixel.l = stickerPixel.l;
              basePixel.a = stickerPixel.a;
            }
          }
        }
      }
      return i;
    }
  }
  return -1;
}

bool StickerSheet::translate (unsigned index, unsigned x, unsigned y) {
  if (*(indexArray_ + index) == 0) {
    return false;
  }
  *baseImage_ = *refImage_;
  for (unsigned i = 0; i < index; i++) {
    addSticker(*(imageArray_ + i), *(imageArray_ + i)->xStart, *(imageArray_ + i)->yStart);
  }
  *(indexArray_ + index) = 0;
  addSticker(*(imageArray_ + index), x, y);
  for (unsigned i = index + 1; i < max_; i++) {
    if (*(indexArray_ + i) != 0) {
      *(indexArray_ + i) = 0;
      addSticker(*(imageArray_ + i), *(imageArray_ + i)->xStart, *(imageArray_ + i)->yStart);
    }
  }
  return true;
}

void StickerSheet::removeSticker (unsigned index) {
  if (*(indexArray_ + index) == 0) {
    return;
  }
  *baseImage_ = *refImage_;
  for (unsigned i = 0; i < max_; i++) {
    if (i == index) {

      *(indexArray_ + i) = 0;
      continue;
    } else {
      if (*(indexArray_ + i) != 0) {
        *(indexArray_ + i) = 0;
        addSticker(*(imageArray_ + i), *(imageArray_ + i)->xStart, *(imageArray_ + i)->yStart);
      } else {
      }
    }
  }
}

Image * StickerSheet::getSticker (unsigned index) {
  if (index < max_  && *(indexArray_ + index) != 0) {
    return (imageArray_ + index);
  }
  return NULL;
}

Image StickerSheet::render() const {
  unsigned finalWidth = (*baseImage_).width();
  unsigned finalHeight = (*baseImage_).height();
  for (unsigned i = 0; i < max_; i++) {
    if (*(imageArray_ + i)->xStart + (*(imageArray_ + i)).width() > finalWidth) {
      finalWidth = *(imageArray_ + i)->xStart + (*(imageArray_ + i)).width();
    }
    if (*(imageArray_ + i)->yStart + (*(imageArray_ + i)).height() > finalHeight) {
      finalHeight = *(imageArray_ + i)->yStart + (*(imageArray_ + i)).height();
    }
    if (finalWidth == (*baseImage_).width() && finalHeight == (*baseImage_).height()) {
      return *baseImage_;
    }
  }
  Image returnImage = Image(finalWidth,finalHeight);
  for (unsigned i = 0; i < (*baseImage_).width(); i++) {
    for (unsigned j = 0; j < (*baseImage_).height(); j++) {
      HSLAPixel & finalPixel = returnImage.getPixel(i,j);
      HSLAPixel & basePixel = baseImage_->getPixel(i,j);
      finalPixel.h = basePixel.h;
      finalPixel.s = basePixel.s;
      finalPixel.l = basePixel.l;
      finalPixel.a = basePixel.a;
    }
  }
  for (unsigned i = 0; i < max_; i++) {
    if (*(indexArray_ + i) != 0) {
      std::cout << (*(imageArray_ + i)).width() << std::endl;
      std::cout << (*(imageArray_ + i)).height() << std::endl;
      for (unsigned a = 0; a < (*(imageArray_ + i)).width(); a++) {
        for (unsigned b = 0; b < (*(imageArray_ + i)).height(); b++) {
          HSLAPixel & basePixel = returnImage.getPixel(*(imageArray_ + i)->xStart + a, *(imageArray_ + i)->yStart + b);
          HSLAPixel & stickerPixel = (*(imageArray_ + i)).getPixel(a,b);
          if (stickerPixel.a > 0) {
            basePixel.h = stickerPixel.h;
            basePixel.s = stickerPixel.s;
            basePixel.l = stickerPixel.l;
            basePixel.a = stickerPixel.a;
          }
        }
      }
    }
  }
  return *baseImage_;
}

void StickerSheet::_copy(const StickerSheet &other) {
  max_ = other.max_;
  indexArray_ = new int[max_]; // delete these!
  baseImage_ = new Image(); // delete these!
  refImage_ = new Image(); // delete these!
  imageArray_ = new Image[max_];
  *baseImage_ = *(other.baseImage_);
  *refImage_ = *(other.refImage_);
  for (unsigned int i = 0; i < max_; i++) {
    *(imageArray_ + i) = *(other.imageArray_ + i);
    *(indexArray_ + i) = *(other.indexArray_ + i);
  }
  *baseImage_ = *refImage_;
  for (unsigned int i = 0; i < max_; i++) {
    if (*(other.indexArray_ + i) != 0) {
      *(indexArray_ + i) = 0;
      addSticker(*(other.imageArray_ + i), *(other.imageArray_ + i)->xStart, *(other.imageArray_ + i)->yStart);
    }
  }
  for (unsigned int i = 0; i < max_; i++) {
  }
}

void StickerSheet::_destroy() {
  delete baseImage_;
  delete refImage_;
  delete[] indexArray_;
  delete[] imageArray_;
}
