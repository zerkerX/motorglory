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
#include "video.h"

#include "timer.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <i86.h>

static enum video_mode video_last_mode = TEXT_MODE;

static uint8_t * CGA1_HW = (uint8_t*)0xB8000000L;
static uint8_t * CGA2_HW = (uint8_t*)0xBA000000L;

uint8_t * CGA1 = (uint8_t*)0xB8000000L;
uint8_t * CGA2 = (uint8_t*)0xBA000000L;

static bool double_buffered = false;

void video_set_mode(enum video_mode mode)
{
    union REGS regs;

    if (mode != video_last_mode)
    {
        switch (mode)
        {
            case CGA_BW:
                regs.h.ah = 0x00;
                regs.h.al = 0x06;
                int86(0x10,&regs,&regs);
                break;
            case CGA_CYAN:
                regs.h.ah = 0x00;
                regs.h.al = 0x04;
                int86(0x10,&regs,&regs);
                break;
            case CGA_YELLOW:
                regs.h.ah = 0x00;
                regs.h.al = 0x04;
                int86(0x10,&regs,&regs);
                regs.h.ah = 0x0B;
                regs.h.bh = 0x01;
                regs.h.bl = 0x00;
                int86(0x10,&regs,&regs);
                break;
            case TEXT_MODE:
                regs.h.ah = 0x00;
                regs.h.al = 0x03;
                int86(0x10,&regs,&regs);
                break;
        }
    }

    video_last_mode = mode;
}

void video_clear()
{
     int i;
     for (i = 0; i < 100; i++)
     {
         memset(CGA1 + 80*i, 0, 80);
         memset(CGA2 + 80*i, 0, 80);
     }
}

void video_clear_region(uint16_t x, uint16_t y,
        uint16_t width, uint16_t height)
{
    int i, j;
    /* j is counting individual rows; two drawn at once */
    for(j = 0; j < height; j += 2)
    {
        /* First row. i is counting full bytes of buffer */
        for (i = 0; i*4 < width; i++)
        {
            /* Only multiply by 40 because j+y is counting every row,
            but this is only drawing every other row */
            CGA1[40*(j+y) + (x/4+i)] = 0;
        }
        /* Second row */
        for (i = 0; i*4 < width; i++)
        {
            CGA2[40*(j+y) + (x/4+i)] = 0;
        }
    }
}


static inline uint8_t fill_byte(uint16_t start, uint16_t end, uint8_t color)
{
    int i;
    uint8_t result = 0;
    if (end > 3) end = 3;
    for ( i = start; i <= end; ++i)
    {
        result |= color << (6 - i * 2);
    }

    return result;
}


void video_draw_box(uint16_t x, uint16_t y,
        uint16_t width, uint16_t height, uint8_t color)
{
    int i, j;
    uint8_t * buffer;
    uint8_t first, mid, last;
    int bytex = x / 4;

    /* For byte width, find byte of last pixel */
    int bytewidth = (x + width - 1) / 4 - bytex + 1; 
    int rowoffs;

    first = fill_byte(x % 4, x % 4 + width - 1, color);
    mid = fill_byte(0, 3, color);
    last = fill_byte(0, (x + width - 1) % 4, color);

    for (j = y; j < y + height; j++)
    {
        if (j % 2 == 0)
        {
            buffer = CGA1;
        }
        else
        {
            buffer = CGA2;
        }

        rowoffs = (j/2)*80;

        for (i = bytex; i < bytex + bytewidth; i++)
        {
            if (i == bytex)
                buffer[rowoffs + i] |= first;
            else if (i == bytex + bytewidth - 1)
                buffer[rowoffs + i] |= last;
            else
                buffer[rowoffs + i] |= mid;
        }
    }
}


void video_enable_double_buffer()
{
    if (!double_buffered)
    {
        CGA1 = (uint8_t *)malloc(VIDEO_BUFFER_SIZE);
        CGA2 = (uint8_t *)malloc(VIDEO_BUFFER_SIZE);
    }
    double_buffered = true;
}


void video_disable_double_buffer()
{
    if (double_buffered)
    {
        free(CGA1);
        free(CGA2);
        CGA1 = CGA1_HW;
        CGA2 = CGA2_HW;
    }
    double_buffered = false;
}


void video_flip_buffer()
{
    if (double_buffered)
    {
        /* Copy by line to avoid comb if the screen refresh catches
        us mid-copy */
        int i;
        for (i = 0; i < 100; i++)
        {
            memcpy(CGA1_HW + 80*i, CGA1 + 80*i, 80);
            memcpy(CGA2_HW + 80*i, CGA2 + 80*i, 80);
        }
    }
}


