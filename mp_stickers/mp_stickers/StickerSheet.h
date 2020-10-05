/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once

#include "Image.h"

class StickerSheet {
  public:
    StickerSheet(const Image::Image& picture, unsigned max);

    StickerSheet(const StickerSheet &other);

    ~StickerSheet();

    const StickerSheet & operator=(const StickerSheet &other);

    void changeMaxStickers (unsigned max);

    int addSticker (Image &sticker, unsigned x, unsigned y);

    bool translate (unsigned index, unsigned x, unsigned y);

    void removeSticker (unsigned index);

    Image * getSticker (unsigned index);

    Image render() const;

    void _copy(const StickerSheet &other);

    void _destroy();

  private:
    unsigned int max_;
    int * indexArray_; // delete these!
    Image * baseImage_; // delete these!
    Image * refImage_; // delete these!
    Image * imageArray_; // delete these!
};
