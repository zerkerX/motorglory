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
#include "bitmap.h"

#include <i86.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bitmap_new(struct bitmap * image,
     unsigned short width, unsigned short height)
{
    int imgsize = (width / 4) * height;

    image->width = width;
    image->height = height;
    image->data = malloc(imgsize);
    memset(image->data, 0, imgsize);
}

void bitmap_load(struct bitmap * image, const char * filename,
     unsigned short width, unsigned short height)
{
    FILE * infile;
    int imgsize = (width / 4) * height;

    image->width = width;
    image->height = height;
    image->data = NULL;

    infile = fopen(filename, "rb");

    if (infile != NULL)
    {
        image->data = malloc(imgsize);
        fread(image->data, 1, imgsize, infile);
        fclose(infile);
    }

}

void bitmap_unload(struct bitmap * image)
{
    if (image->data != NULL)
    {
        free(image->data);
        image->data = NULL;
    }
}


void bitmap_draw(struct bitmap * image, unsigned short x, unsigned short y)
{
    int i, j;
    /* j is counting individual rows; two drawn at once */
    for(j = 0; j < image->height; j += 2)
    {
        /* First row. i is counting full bytes of buffer */
        for (i = 0; i*4 < image->width; i++)
        {
            /* Only multiply by 40 because j+y is counting every row,
            but this is only drawing every other row */
            CGA1[40*(j+y) + (x/4+i)] |= image->data[image->width/4*j+i];
        }
        /* Second row */
        for (i = 0; i*4 < image->width; i++)
        {
            CGA2[40*(j+y) + (x/4+i)] |= image->data[image->width/4*(j+1)+i];
        }
    }
}


void bitmap_mask(struct bitmap * image, unsigned short x, unsigned short y)
{
    int i, j;
    /* j is counting individual rows; two drawn at once */
    for(j = 0; j < image->height; j += 2)
    {
        /* First row. i is counting full bytes of buffer */
        for (i = 0; i*4 < image->width; i++)
        {
            /* Only multiply by 40 because j+y is counting every row,
            but this is only drawing every other row */
            CGA1[40*(j+y) + (x/4+i)] &= image->data[image->width/4*j+i];
        }
        /* Second row */
        for (i = 0; i*4 < image->width; i++)
        {
            CGA2[40*(j+y) + (x/4+i)] &= image->data[image->width/4*(j+1)+i];
        }
    }
}


void bitmap_replace(struct bitmap * image, unsigned short x, unsigned short y)
{
    int i, j;
    /* j is counting individual rows; two drawn at once */
    for(j = 0; j < image->height; j += 2)
    {
        /* First row. i is counting full bytes of buffer */
        for (i = 0; i*4 < image->width; i++)
        {
            /* Only multiply by 40 because j+y is counting every row,
            but this is only drawing every other row */
            CGA1[40*(j+y) + (x/4+i)] = image->data[image->width/4*j+i];
        }
        /* Second row */
        for (i = 0; i*4 < image->width; i++)
        {
            CGA2[40*(j+y) + (x/4+i)] = image->data[image->width/4*(j+1)+i];
        }
    }
}



void bitmap_display_file(const char * filename, enum video_mode mode)
{
    char buf[ 80 ];
    FILE * infile;
    int i;

    infile = fopen(filename, "rb");

    if (infile != NULL)
    {
        video_set_mode(mode);

        for (i = 0; i < 100; i++)
        {
            fread(buf, 1, 80, infile);
            memcpy(CGA1 + 80*i, buf, 80);
            fread(buf, 1, 80, infile);
            memcpy(CGA2 + 80*i, buf, 80);
        }
        fclose(infile);
    }
}

