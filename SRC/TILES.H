/* This file is part of Motor Glory.

Motor Glory is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Motor Glory is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Motor Glory.  If not, see <https://www.gnu.org/licenses/>.
*/
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
