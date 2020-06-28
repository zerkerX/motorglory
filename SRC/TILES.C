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
#include "tiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <i86.h>

void tiles_load(struct tiles * tiles, const char * filename,
        unsigned short width, unsigned short height,
        unsigned short numx, unsigned short numy)
{
    int i, j, k;
    FILE * infile;
    tiles->num = numx*numy;
    tiles->tile = malloc(sizeof(struct bitmap) * tiles->num);

    for (i = 0; i < tiles->num; i++)
    {
        bitmap_new(&tiles->tile[i], width, height);
    }

    infile = fopen(filename, "rb");

    if (infile != NULL)
    {
        /* numy rows of tiles */
        for (k = 0; k < numy; k++)
        {
            /* height rows per tile */
            for (j = 0; j < height; j++)
            {
                /* numx tiles per row */
                for (i = 0; i < numx; i++)
                {
                    /* width/4 bytes per tile */
                    fread(&tiles->tile[numx*k+i].data[j*(width/4)],
                            1, width/4, infile);
                }
            }
        }
        fclose(infile);
    }


}

void tiles_unload(struct tiles * tiles)
{
    int i;

    for (i = 0; i < tiles->num; i++)
    {
        bitmap_unload(&tiles->tile[i]);
    }

    free(tiles->tile);
}


void tiles_draw(struct tiles * tiles, int tilenum,
        unsigned short x, unsigned short y)
{
    bitmap_draw(&tiles->tile[tilenum], x, y);
}


void tiles_mask(struct tiles * tiles, int tilenum,
        unsigned short x, unsigned short y)
{
    bitmap_mask(&tiles->tile[tilenum], x, y);
}


void tiles_replace(struct tiles * tiles, int tilenum,
        unsigned short x, unsigned short y)
{
    bitmap_replace(&tiles->tile[tilenum], x, y);
}
