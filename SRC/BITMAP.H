#ifndef BITMAP_H
#define BITMAP_H

#include "video.h"

struct bitmap
{
    unsigned short width;
    unsigned short height;
    unsigned char * data;
};

void bitmap_new(struct bitmap * image,
     unsigned short width, unsigned short height);

void bitmap_load(struct bitmap * image, const char * filename,
     unsigned short width, unsigned short height);

void bitmap_unload(struct bitmap * image);

void bitmap_draw(struct bitmap * image, unsigned short x, unsigned short y);

void bitmap_mask(struct bitmap * image, unsigned short x, unsigned short y);

void bitmap_replace(struct bitmap * image, unsigned short x, unsigned short y);

void bitmap_display_file(const char * filename, enum video_mode mode);

#endif
