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
