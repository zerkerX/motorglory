#ifndef TILES_H
#define TILES_H

#include "bitmap.h"

struct tiles
{
    unsigned short num;
    struct bitmap * tile;
};

void tiles_load(struct tiles * tiles, const char * filename,
        unsigned short width, unsigned short height,
        unsigned short numx, unsigned short numy);

void tiles_unload(struct tiles * tiles);

void tiles_draw(struct tiles * tiles, int charnum,
        unsigned short x, unsigned short y);

void tiles_mask(struct tiles * tiles, int charnum,
        unsigned short x, unsigned short y);

void tiles_replace(struct tiles * tiles, int tilenum,
        unsigned short x, unsigned short y);

#endif
